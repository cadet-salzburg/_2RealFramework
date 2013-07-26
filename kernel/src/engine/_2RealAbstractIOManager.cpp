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
#include "engine/_2RealParameterMetadata.h"
#include "engine/_2RealInletBasedTrigger.h"
#include "engine/_2RealLogger.h"
#include "engine/_2RealEngineImpl.h"

#include <assert.h>

using std::string;

namespace _2Real
{

	AbstractInletIO::AbstractInletIO( EngineImpl *engine, AbstractUberBlock *owner, AbstractUpdatePolicy *policy, IOInfo *info ) :
		NonCopyable< AbstractInletIO >(),
		Identifiable< AbstractInletIO >( owner->getIds(), info->name ),
		Handleable< AbstractInletIO, app::InletHandle >( *this ),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mPolicy( policy ),
		mInfo( info )
	{
#ifdef _DEBUG
		assert( mEngineImpl );
		assert( mOwningBlock );
		assert( mPolicy );
#endif
	}

	IOInfo * AbstractInletIO::getInfo()
	{
		return mInfo;
	}

	AbstractUberBlock * AbstractInletIO::getOwningBlock()
	{
		return mOwningBlock;
	}

	bool AbstractInletIO::belongsToBlock( AbstractUberBlock const* block ) const
	{
		return mOwningBlock == block;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BasicInletIO::BasicInletIO( EngineImpl *engine, AbstractUberBlock *owner, AbstractUpdatePolicy *policy, IOInfo *info ) :
		AbstractInletIO( engine, owner, policy, info ),
		mInlet( new BasicInlet( this ) ),
		mBuffer( new BasicInletBuffer( this ) ),
		mQueue( new DataQueue( this, 10 ) ),
		mUpdateTrigger( nullptr ),					// will be initialized later on ( POSSIBLE CRASH!!! )
		mNotifyOnReceive( false )					// in the beginning, everything will be written into the buffer
	{
	}

	BasicInletIO::~BasicInletIO()
	{
		delete mInlet;
		delete mBuffer;
		delete mQueue;
		delete mInfo;
	}

	bundle::InletHandle & BasicInletIO::getBundleHandle() const
	{
		return mInlet->getHandle();
	}

	void BasicInletIO::setQueueSize( const unsigned int size )
	{
		mQueue->setMaxCapacity( size );
	}

	void BasicInletIO::setUpdatePolicy( Policy const& p )
	{
		mInfo->policy = p;
		mPolicy->setInletPolicy( *this, p );
	}

	void BasicInletIO::setData( std::shared_ptr< const CustomType > data )
	{
		mBuffer->setData( data );
	}

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

			bool wasTriggered = mUpdateTrigger->tryFulfillCondition( data );	// test the update condition
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

	void BasicInletIO::synchronizeData()
	{
		mEngineImpl->getLogger()->addLine( getFullName() + "\t-------synchronizing-----" );
		std::shared_ptr< const CustomType > newData = mBuffer->getData();
		mInlet->update( newData );
		mEngineImpl->getLogger()->addLine( getFullName() + "\t-------synchronized------" );
	}

	void BasicInletIO::processQueue()
	{
		mEngineImpl->getLogger()->addLine( getFullName() + "\t-------processing queue-----" );

		// at this point, mNotifyOnReceive is still false so no mutex is needed - incoming data is just written into the buffer
		bool wasTriggered = false;
		TimestampedData data;
		while( mQueue->getDataItem( data ) )
		{
			wasTriggered = mUpdateTrigger->tryFulfillCondition( data );			// test the update condition
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

	const std::string BasicInletIO::getQueueSizeAsString() const
	{
		std::ostringstream str;
		str << mQueue->getMaxCapacity();
		return str.str();
	}

	const std::string BasicInletIO::getUpdatePolicyAsString() const
	{
		return Policy::getPolicyAsString( mInfo->policy.getPolicy() );
	}

	const std::string BasicInletIO::getCurrentDataAsString() const
	{
		std::ostringstream str;
		std::shared_ptr< const CustomType > data = mInlet->getDataThreadsafe();
		if ( data.get() == nullptr ) str << "empty" << std::endl;
		else data->writeTo( str );
		return str.str();
	}

	std::shared_ptr< const CustomType > BasicInletIO::getCurrentData() const
	{
		std::shared_ptr< const CustomType > data = mInlet->getDataThreadsafe();
		return data;
	}

	bool BasicInletIO::linkTo( OutletIO *outlet )
	{
		return mEngineImpl->createLink( *this, *outlet ).isValid();
	}

	void BasicInletIO::unlinkFrom( OutletIO *outlet )
	{
		mEngineImpl->destroyLink( *this, *outlet );
	}

	void BasicInletIO::setTrigger( AbstractInletBasedTrigger *trigger )
	{
		mUpdateTrigger = trigger;
	}

	void BasicInletIO::removeTrigger( AbstractInletBasedTrigger *trigger )
	{
		mUpdateTrigger = nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MultiInletIO::MultiInletIO( EngineImpl *engine, AbstractUberBlock *owner, AbstractUpdatePolicy *policy, IOInfo *info ) :
		AbstractInletIO( engine, owner, policy, info ),
		mInlet( new MultiInlet( this ) )
	{
	}

	MultiInletIO::~MultiInletIO()
	{
		for ( std::vector< IO >::iterator it = mBasicInletIOs.begin(); it != mBasicInletIOs.end(); ++it )
			delete ( *it ).io;

		for ( std::list< BasicInletIO * >::iterator it = mTemporaryInletIOs.begin(); it != mTemporaryInletIOs.end(); ++it )
			delete *it;

		delete mInlet;
		delete mInfo;
	}

	BasicInletIO & MultiInletIO::operator[]( const unsigned int index )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		try
		{
			return *( mBasicInletIOs.at( index ).io );
		}
		catch ( std::out_of_range &e )
		{
			throw Exception( e.what() );
		}
	}

	bundle::InletHandle & MultiInletIO::getBundleHandle() const
	{
		return mInlet->getHandle();
	}

	AbstractInletIO * MultiInletIO::addBasicInlet()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		IOInfo *info = new IOInfo( *mInfo );
		BasicInletIO *io = new BasicInletIO( mEngineImpl, mOwningBlock, mPolicy, info );
		mTemporaryInletIOs.push_back( io );
		mPolicy->addInlet( *io, info->policy );		// why?
		return io;
	}

	void MultiInletIO::removeBasicInlet( AbstractInletIO *io )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		// check tmp list first
		for ( std::list< BasicInletIO * >::iterator it = mTemporaryInletIOs.begin(); it != mTemporaryInletIOs.end(); ++it )
		{
			if ( io == *it )
			{
				it = mTemporaryInletIOs.erase( it );
				mPolicy->removeInlet( **it );
				return;
			}
		}

		for ( std::vector< IO >::iterator it = mBasicInletIOs.begin(); it != mBasicInletIOs.end(); ++it )
		{
			if ( io == ( *it ).io )
			{
				mEngineImpl->clearLinksFor( *( ( *it ).io ) );	// kill all links involving this inlet
				mPolicy->removeInlet( *( *it ).io );			// inlet should not influence updates any more
				( *it ).wasRemoved = true;						// but inlet must stay alive, since it might still be accessed
				return;
			}
		}
	}

	void MultiInletIO::synchronizeData()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		// move tmp inlets over
		for ( std::list< BasicInletIO * >::iterator it = mTemporaryInletIOs.begin(); it != mTemporaryInletIOs.end(); ++it )
		{
			BasicInletIO *io = *it;
			std::ostringstream name;
			name << mInfo->name << "::" << mBasicInletIOs.size();
			io->getInfo()->name = name.str();
			mBasicInletIOs.push_back( IO( io ) );
		}

		mTemporaryInletIOs.clear();

		for ( std::vector< IO >::iterator it = mBasicInletIOs.begin(); it != mBasicInletIOs.end(); ++it )
		{
			BasicInletIO *io = ( *it ).io;
			if ( ( *it ).wasRemoved )
			{
				delete io;
				io = nullptr;
				// argh
			}
			else
				io->synchronizeData();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ParameterIO::ParameterIO( EngineImpl *engine, AbstractUberBlock *owner, IOInfo *info ) :
		Identifiable< ParameterIO >( owner->getIds(), info->name ),
		Handleable< ParameterIO, app::ParameterHandle >( *this ),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mParameter( new Parameter( this ) ),
		mBuffer( new ParameterBuffer( this ) ),
		mInfo( info )
	{
	}

	ParameterIO::~ParameterIO()
	{
		delete mParameter;
		delete mBuffer;
		delete mInfo;
	}

	IOInfo * ParameterIO::getInfo()
	{
		return mInfo;
	}

	AbstractUberBlock * ParameterIO::getOwningBlock()
	{
		return mOwningBlock;
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

	bundle::ParameterHandle ParameterIO::getBundleHandle() const
	{
		return mParameter->getHandle();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	OutletIO::OutletIO( EngineImpl *engine, AbstractUberBlock *owner, IOInfo *info ) :
		Identifiable< OutletIO >( owner->getIds(), info->name ),
		Handleable< OutletIO, app::OutletHandle >( *this ),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mOutlet( new Outlet( this ) ),
		mBuffer( new OutletBuffer( this ) ),
		mInfo( info ),
		mAppEvent( new CallbackEvent< std::shared_ptr< const CustomType > >() ),
		mInletEvent( new CallbackEvent< TimestampedData const& >() )
	{
	}

	OutletIO::~OutletIO()
	{
		delete mOutlet;
		delete mBuffer;
		delete mAppEvent;
		delete mInletEvent;
		delete mInfo;
	}

	IOInfo * OutletIO::getInfo()
	{
		return mInfo;
	}

	AbstractUberBlock * OutletIO::getOwningBlock()
	{
		return mOwningBlock;
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

	bool OutletIO::linkTo( BasicInletIO &inlet )
	{
		return mEngineImpl->createLink( inlet, *this ).isValid();
	}

	void OutletIO::unlinkFrom( BasicInletIO &inlet )
	{
		mEngineImpl->destroyLink( inlet, *this );
	}

	bundle::OutletHandle OutletIO::getBundleHandle() const
	{
		return mOutlet->getHandle();
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