/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "engine/_2RealAbstractUpdatePolicy.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealIOMetadata.h"
#include "engine/_2RealInletBasedTrigger.h"
#include "engine/_2RealLogger.h"
#include "engine/_2RealEngineImpl.h"

using std::string;

namespace _2Real
{

	AbstractInletIO::AbstractInletIO( EngineImpl *engine, AbstractUberBlock *owner, AbstractUpdatePolicy *policy, std::shared_ptr< const IOMetadata > meta ) :
		NonCopyable< AbstractInletIO >(),
		Identifiable< AbstractInletIO >( owner->getIds(), meta->name ),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mPolicy( policy ),
		mMetadata( meta )
	{
	}

	std::shared_ptr< const IOMetadata > AbstractInletIO::getInfo() const
	{
		return mMetadata;
	}

	bool AbstractInletIO::belongsToBlock( AbstractUberBlock const* block ) const
	{
		return mOwningBlock == block;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BasicInletIO::BasicInletIO( EngineImpl *engine, AbstractUberBlock *owner, AbstractUpdatePolicy *policy, std::shared_ptr< const IOMetadata > meta ) :
		AbstractInletIO( engine, owner, policy, meta ),
		mInlet( new BasicInlet( this ) ),
		mBuffer( new BasicInletBuffer( this ) ),
		mQueue( new DataQueue( this, 10 ) ),
		mUpdateTrigger( nullptr ),					// will be initialized later on ( POSSIBLE CRASH!!! )
		mNotifyOnReceive( false )					// in the beginning, everything will be written into the buffer
	{
		policy->addInlet( *this, meta->updatePolicy );				// creates a trigger
		setData( meta->initializer );								// set buffer to init value
		synchronizeData();											// inlet now holds init value
		synchronizeData();											// hasChanged() == false
		//receiveData( TimestampedData( meta->initializer, -1 ) );	// write data into queue as well -> initializes the cond
		//															// first timestamp = 0, b/c this way all regularly sent items ar newer

		mInlet->setSelfRef( mInlet );
	}

	void BasicInletIO::setSelfRef( std::shared_ptr< BasicInletIO > ref )
	{
		mSelfRef = ref;
	}

	std::shared_ptr< BasicInletIO > BasicInletIO::getSelfRef()
	{
		std::shared_ptr< BasicInletIO > locked = mSelfRef.lock();
		return locked;
	}

	std::shared_ptr< const BasicInletIO > BasicInletIO::getSelfRef() const
	{
		std::shared_ptr< BasicInletIO > locked = mSelfRef.lock();
		return locked;
	}

	////--------------------- multiinlet

	bool BasicInletIO::canExpand() const
	{
		return false;
	}

	unsigned int BasicInletIO::getSubInletCount() const
	{
		return 1;
	}

	std::shared_ptr< BasicInletIO > BasicInletIO::operator[]( const unsigned int index )
	{
		( void )( index );
		std::shared_ptr< BasicInletIO > locked = mSelfRef.lock();
		return locked;
	}

	std::shared_ptr< AbstractInletIO > BasicInletIO::addSubInlet()
	{
		return std::shared_ptr< AbstractInletIO >();
	}

	void BasicInletIO::removeSubInlet( std::shared_ptr< AbstractInletIO > io )
	{
		( void )( io );
	}

	void BasicInletIO::syncInletChanges()
	{
	}

	////--------------------- multiinlet

	std::shared_ptr< AbstractInlet > BasicInletIO::getInlet()
	{
		return mInlet;
	}

	std::shared_ptr< const AbstractInlet > BasicInletIO::getInlet() const
	{
		return mInlet;
	}

	////--------------------- queue

	bool BasicInletIO::isBuffered() const
	{
		return mMetadata->isBuffered;
	}

	void BasicInletIO::setQueueSize( const unsigned int size )
	{
		if ( isBuffered() )
			mQueue->setMaxCapacity( size );
	}

	void BasicInletIO::processQueue()
	{
		if ( !isBuffered() ) return;

		mEngineImpl->getLogger()->addLine( getFullName() + "\t-------processing queue-----" );

		// at this point, mNotifyOnReceive is still false so no mutex is needed - incoming data is just written into the buffer
		bool wasTriggered = false;
		TimestampedData data;
		while( mQueue->getDataItem( data ) )
		{
			wasTriggered = mUpdateTrigger->tryFulfillCondition( data, false );	// test the update condition
																				// if update cond fulfilled, trigger will:
																				//	-set its status to 'fulfilled'
																				//	-store the data, for the next trigger attempt
			if ( wasTriggered ) break;
		}

		mEngineImpl->getLogger()->addLine( getFullName() + "\t-------processed queue------" );

		// if no buffered data item managed to trigger, enable mNotifyOnReceive
		if ( !wasTriggered )
		{
			mAccess.lock();
			mNotifyOnReceive = true;
			mAccess.unlock();
			mEngineImpl->getLogger()->addLine( getFullName() + "\tdid not fulfill cond from queue, try with last value" );

			// try trigger with data of last update: -1 makes sure that 'newer timestamp' can never be fulfilled, but 'valid data' will be
			receiveData( TimestampedData( mBuffer->getData(), -1 ) );
		}
		else
		{
			mBuffer->setData( data.value );
			mEngineImpl->getLogger()->addLine( getFullName() + "\tdid fulfill cond from queue, try trigger" );
			mUpdateTrigger->tryTriggerUpdate();
		}
	}

	////--------------------- queue

	////--------------------- triggering

	bool BasicInletIO::canTriggerUpdates() const
	{
		return mMetadata->canTriggerUpdates;
	}

	void BasicInletIO::setUpdatePolicy( UpdatePolicy const& p )
	{
		if ( canTriggerUpdates() )
			mPolicy->setInletPolicy( *this, p );
	}

	void BasicInletIO::setTrigger( AbstractInletBasedTrigger *trigger )
	{
		if ( canTriggerUpdates() )
			mUpdateTrigger = trigger;
	}

	void BasicInletIO::removeTrigger( AbstractInletBasedTrigger *trigger )
	{
		mUpdateTrigger = nullptr;
	}

	////--------------------- triggering

	////--------------------- data

	void BasicInletIO::receiveData( std::shared_ptr< const CustomType > data )
	{
		receiveData( TimestampedData( data, mEngineImpl->getElapsedTime() ) );
	}

	void BasicInletIO::receiveData( TimestampedData const& data )
	{
		mEngineImpl->getLogger()->addLine( getFullName() + "\treceived data" );

		mAccess.lock();
		if ( !mNotifyOnReceive )
		{
			mAccess.unlock();
			mQueue->storeDataItem( data );
			mEngineImpl->getLogger()->addLine( getFullName() + "\tqueued data" );
		}
		else
		{
			mEngineImpl->getLogger()->addLine( getFullName() + "\ttry" );

			bool wasTriggered = mUpdateTrigger->tryFulfillCondition( data, false );	// test the update condition
																					// if update cond fulfilled, trigger will:
																					//	-set its status to 'fulfilled'
																					//	-store the data, for the next trigger attempt

			if ( wasTriggered )
			{
				mEngineImpl->getLogger()->addLine( getFullName() + "\tfulfilled cond" );
				mBuffer->setData( data.value );
				mNotifyOnReceive = false;
				mUpdateTrigger->tryTriggerUpdate();
			}
			mAccess.unlock();
		}
	}

	void BasicInletIO::setData( std::shared_ptr< const CustomType > data )
	{
		TimestampedData timestamped( data, mEngineImpl->getElapsedTime() );
		mEngineImpl->getLogger()->addLine( getFullName() + "\tset data" );

		mAccess.lock();
		bool wasTriggered = mUpdateTrigger->tryFulfillCondition( timestamped, true );		// test the update condition
																							// if update cond fulfilled, trigger will:
																							//	-set its status to 'fulfilled'
																							//	-store the data, for the next trigger attempt

		if ( wasTriggered )
		{
			mEngineImpl->getLogger()->addLine( getFullName() + "\tfulfilled cond" );
			mNotifyOnReceive = false;
			mUpdateTrigger->tryTriggerUpdate();
		}

		mBuffer->setData( data );
		mAccess.unlock();
	}

	void BasicInletIO::synchronizeData()
	{
		mEngineImpl->getLogger()->addLine( getFullName() + "\t-------synchronizing-----" );
		std::shared_ptr< const CustomType > newData = mBuffer->getData();
		mInlet->update( newData );
		mEngineImpl->getLogger()->addLine( getFullName() + "\t-------synchronized------" );
	}

	std::shared_ptr< const CustomType > BasicInletIO::getCurrentData() const
	{
		std::shared_ptr< const CustomType > data = mInlet->getDataThreadsafe();
		return data;
	}

	////--------------------- data

	////--------------------- linking

	bool BasicInletIO::canLink() const
	{
		return mMetadata->canLink;
	}

	std::shared_ptr< IOLink > BasicInletIO::linkTo( std::shared_ptr< OutletIO > outlet )
	{
		if ( !canLink() ) return std::shared_ptr< IOLink >();
		std::shared_ptr< IOLink > link = mEngineImpl->getLinkManager()->createLink( getSelfRef(), outlet );
		return link;
	}

	void BasicInletIO::unlinkFrom( std::shared_ptr< OutletIO > outlet )
	{
		if ( !canLink() ) return;
		mEngineImpl->getLinkManager()->destroyLink( getSelfRef(), outlet );
	}

	////--------------------- linking

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MultiInletIO::MultiInletIO( EngineImpl *engine, AbstractUberBlock *owner, AbstractUpdatePolicy *policy, std::shared_ptr< const IOMetadata > meta ) :
		AbstractInletIO( engine, owner, policy, meta ),
		mInlet( new MultiInlet( this ) )
	{
	}

	MultiInletIO::~MultiInletIO()
	{
	}

	std::shared_ptr< BasicInletIO > MultiInletIO::operator[]( const unsigned int index )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		try
		{
			return mBasicInletIOs.at( index ).io;
		}
		catch ( std::out_of_range const& e )
		{
			throw Exception( e.what() );
		}
	}

	std::shared_ptr< AbstractInletIO > MultiInletIO::addSubInlet()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		std::shared_ptr< IOMetadata > meta( new IOMetadata( *mMetadata.get() ) );
		BasicInletIO *io = new BasicInletIO( mEngineImpl, mOwningBlock, mPolicy, meta );
		std::shared_ptr< BasicInletIO > shared( io );
		io->setSelfRef( shared );
		mTemporaryInletIOs.push_back( shared );
		mPolicy->addInlet( *io, meta->updatePolicy );		// why?
		return std::static_pointer_cast< AbstractInletIO, BasicInletIO >( shared );
	}

	std::shared_ptr< AbstractInlet > MultiInletIO::getInlet()
	{
		return mInlet;
	}

	std::shared_ptr< const AbstractInlet > MultiInletIO::getInlet() const
	{
		return mInlet;
	}

	void MultiInletIO::removeSubInlet( std::shared_ptr< AbstractInletIO > io )
	{
		//Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		//// check tmp list first
		//for ( std::list< std::shared_ptr< BasicInletIO > >::iterator it = mTemporaryInletIOs.begin(); it != mTemporaryInletIOs.end(); ++it )
		//{
		//	if ( io == ( *it ) )
		//	{
		//		it = mTemporaryInletIOs.erase( it );
		//		mPolicy->removeInlet( **it );
		//		return;
		//	}
		//}

		//for ( std::vector< IO >::iterator it = mBasicInletIOs.begin(); it != mBasicInletIOs.end(); ++it )
		//{
		//	if ( io == ( *it ).io )
		//	{
		//		mEngineImpl->clearLinksFor( *( ( *it ).io ) );	// kill all links involving this inlet
		//		mPolicy->removeInlet( *( *it ).io );			// inlet should not influence updates any more
		//		( *it ).wasRemoved = true;						// but inlet must stay alive, since it might still be accessed
		//		return;
		//	}
		//}
	}

	void MultiInletIO::synchronizeData()
	{
		//Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		//// move tmp inlets over
		//for ( std::list< std::shared_ptr< BasicInletIO > >::iterator it = mTemporaryInletIOs.begin(); it != mTemporaryInletIOs.end(); ++it )
		//{
		//	BasicInletIO *io = *it;
		//	std::ostringstream name;
		//	name << mInfo->name << "::" << mBasicInletIOs.size();
		//	io->getInfo()->name = name.str();
		//	mBasicInletIOs.push_back( IO( io ) );
		//}

		//mTemporaryInletIOs.clear();

		//for ( std::vector< IO >::iterator it = mBasicInletIOs.begin(); it != mBasicInletIOs.end(); ++it )
		//{
		//	BasicInletIO *io = ( *it ).io;
		//	if ( ( *it ).wasRemoved )
		//	{
		//		delete io;
		//		io = nullptr;
		//		// argh
		//	}
		//	else
		//		io->synchronizeData();
		//}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ParameterIO::ParameterIO( EngineImpl *engine, AbstractUberBlock *owner, std::shared_ptr< const IOMetadata > meta ) :
		Identifiable< ParameterIO >( owner->getIds(), meta->name ),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mParameter( new Parameter( this ) ),
		mBuffer( new ParameterBuffer( this ) ),
		mInfo( meta )
	{
	}

	ParameterIO::~ParameterIO()
	{
		delete mBuffer;
	}

	std::shared_ptr< const IOMetadata > ParameterIO::getInfo() const
	{
		return mInfo;
	}

	bool ParameterIO::belongsToBlock( AbstractUberBlock const* block ) const
	{
		return mOwningBlock == block;
	}

	std::shared_ptr< const CustomType > ParameterIO::getCurrentDataThreadsafe() const
	{
		// called from app, always reads the buffer + mutex
		return mParameter->getDataThreadsafe();
	}

	void ParameterIO::setData( std::shared_ptr< const CustomType > newData )
	{
		// called from app, always reads the buffer + mutex
		return mBuffer->setData( newData );
	}

	void ParameterIO::synchronizeData()
	{
		std::shared_ptr< const CustomType > newData = mBuffer->getData();
		mParameter->update( newData );
	}

	std::shared_ptr< Parameter > ParameterIO::getParameter()
	{
		return mParameter;
	}

	std::shared_ptr< const Parameter > ParameterIO::getParameter() const
	{
		return mParameter;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	OutletIO::OutletIO( EngineImpl *engine, AbstractUberBlock *owner, std::shared_ptr< const IOMetadata > meta ) :
		Identifiable< OutletIO >( owner->getIds(), meta->name ),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mOutlet( new Outlet( this ) ),
		mBuffer( new OutletBuffer( this ) ),
		mInfo( meta ),
		mAppEvent( new CallbackEvent< std::shared_ptr< const CustomType > >() ),
		mInletEvent( new CallbackEvent< TimestampedData const& >() )
	{
	}

	OutletIO::~OutletIO()
	{
		delete mBuffer;
		delete mAppEvent;
		delete mInletEvent;
	}

	std::shared_ptr< OutletIO > OutletIO::getSelfRef()
	{
		std::shared_ptr< OutletIO > locked = mSelfRef.lock();
		return locked;
	}

	std::shared_ptr< const OutletIO > OutletIO::getSelfRef() const
	{
		std::shared_ptr< OutletIO > locked = mSelfRef.lock();
		return locked;
	}

	std::shared_ptr< const IOMetadata > OutletIO::getInfo() const
	{
		return mInfo;
	}

	bool OutletIO::belongsToBlock( AbstractUberBlock const* block ) const
	{
		return mOwningBlock == block;
	}

	std::shared_ptr< const CustomType > OutletIO::getCurrentDataThreadsafe() const
	{
		// called from app, always reads the buffer + mutex
		return mBuffer->getData();
	}

	std::shared_ptr< const CustomType > OutletIO::synchronizeData()
	{
		// clone initializer
		std::shared_ptr< CustomType > newData( new CustomType( *( mInfo->initializer.get() ) ) ) ;
		// retrieve old data from outlet & set new data
		std::shared_ptr< const CustomType > recentData = mOutlet->update( newData );
		// store old data in buffer
		mBuffer->setData( recentData );
		// notify: only if not empty
		if ( recentData.get() )
		{
			mInletEvent->notify( TimestampedData( recentData, mEngineImpl->getElapsedTime() ) );
			mAppEvent->notify( recentData );
		}
		return recentData;
	}

	std::shared_ptr< IOLink > OutletIO::linkTo( std::shared_ptr< BasicInletIO > inlet )
	{
		std::shared_ptr< IOLink > link = mEngineImpl->getLinkManager()->createLink( inlet, getSelfRef() );
		return link;
	}

	void OutletIO::unlinkFrom( std::shared_ptr< BasicInletIO > inlet )
	{
		mEngineImpl->getLinkManager()->destroyLink( inlet, getSelfRef() );
	}

	std::shared_ptr< Outlet > OutletIO::getOutlet()
	{
		return mOutlet;
	}

	std::shared_ptr< const Outlet > OutletIO::getOutlet() const
	{
		return mOutlet;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	AbstractIOManager::AbstractIOManager( EngineImpl *engine, AbstractUberBlock *owner ) :
		mEngineImpl( engine ),
		mOwner( owner )
	{
	}

	AbstractIOManager::~AbstractIOManager()
	{
	}

	std::string AbstractIOManager::getName() const
	{
		return mOwner->getFullName();
	}

}