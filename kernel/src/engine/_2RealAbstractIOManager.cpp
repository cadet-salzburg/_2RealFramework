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
#include "engine/_2RealFunctionBlockUpdateManager.h"
#include "engine/_2RealAbstractUpdateManager.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealIOMetadata.h"
#include "engine/_2RealLogger.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealAbstractDataCondition.h"

using std::string;

namespace _2Real
{

	BasicInletIO::BasicInletIO( EngineImpl *engine, std::shared_ptr< InstanceId > id, AbstractUberBlock *owner, std::shared_ptr< AbstractUpdateManager > updateMgr, std::shared_ptr< const IOMetadata > meta ) :
		AbstractInletIO(),
		mInlet( new BasicInlet( this ) ),
		mBuffer( new BasicInletBuffer( this ) ),
		mQueue( nullptr ),
		mEngineImpl( engine ),
		mUpdateManager( updateMgr ),
		mOwningBlock( owner ),
		mMetadata( meta ),
		mIdentifier( id ),
		mClearBuffer( false )
	{
		if ( meta->isBuffered )
			mQueue.reset( new DataQueue( this, meta->expansionSize ) );
		else
			mQueue.reset( new DataQueue( this, 1 ) );

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

	std::string const& BasicInletIO::getFullHumanReadableName() const
	{
		return mIdentifier->getFullHumanReadableName();
	}

	std::string const& BasicInletIO::getHumanReadableName() const
	{
		return mIdentifier->getHumanReadableName();
	}

	//std::string const& BasicInletIO::getCode() const
	//{
	//	return mIdentifier.getCode();
	//}

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

	////--------------------- multiinlet

	std::shared_ptr< AbstractInlet > BasicInletIO::getInlet()
	{
		return mInlet;
	}

	std::shared_ptr< const AbstractInlet > BasicInletIO::getInlet() const
	{
		return mInlet;
	}

	std::shared_ptr< const IOMetadata > BasicInletIO::getMetadata() const
	{
		return mMetadata;
	}

	bool BasicInletIO::belongsToBlock( AbstractUberBlock const* block ) const
	{
		return mOwningBlock == block;
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

	////--------------------- queue

	////--------------------- triggering

	bool BasicInletIO::canTriggerUpdates() const
	{
		return mMetadata->canTriggerUpdates;
	}

	bool BasicInletIO::canFulfillCondition( std::shared_ptr< ADataCondition > cond ) const
	{
		//Poco::ScopedLock< Poco::FastMutex > lock( mBufferAccess );

		// try with the data in the buffer itself
		TimestampedData data = mBuffer->getData();
		if ( cond->canFulfill( data ) )
		{
			return true;
		}

		// then with the queue
		while ( mQueue->getDataItem( data, DataQueue::KEEP ) )
		{
			if ( cond->canFulfill( data ) )
			{
				return true;
			}
		}

		// then with the last daat
		if ( cond->canFulfill( mLastData ) )
		{
			return true;
		}

		// then, with the default value
		data.timestamp = ( std::numeric_limits< long >::min )();
		data.value = mMetadata->initializer;
		if ( cond->canFulfill( data ) )
		{
			return true;
		}

		return false;
	}

	// only called if I know for sure that the inlets can fullfill their respective conditions
	// however, this might still fail, because the user might have something in the meanwhile
	bool BasicInletIO::tryFulfillCondition( std::shared_ptr< ADataCondition > cond )
	{
		//Poco::ScopedLock< Poco::FastMutex > lock( mBufferAccess );

		TimestampedData data = mBuffer->getData();
		if ( cond->tryFulfill( data, true ) )
		{
			mBuffer->setData( data );
			return true;
		}

		while ( mQueue->getDataItem( data, DataQueue::DISCARD ) )
		{
			if ( cond->tryFulfill( data, true ) )
			{
				mBuffer->setData( data );
				return true;
			}
		}

		if ( cond->tryFulfill( mLastData, true ) )
		{
			mClearBuffer = true;
			mBuffer->setData( mLastData );
			return true;
		}

		data.timestamp = ( std::numeric_limits< long >::min )();
		data.value = mMetadata->initializer;
		if ( cond->tryFulfill( data, true ) )
		{
			mClearBuffer = true;
			mBuffer->setData( data );
			return true;
		}

		return false;
	}

	////--------------------- triggering

	////--------------------- data

	void BasicInletIO::receiveData( std::shared_ptr< const CustomType > data )
	{
		receiveData( TimestampedData( data, mEngineImpl->getElapsedTime() ) );
	}

	void BasicInletIO::receiveData( TimestampedData const& data )
	{
		//Poco::ScopedLock< Poco::FastMutex > lock( mBufferAccess );

		mQueue->storeDataItem( data );
		if ( canTriggerUpdates() ) mUpdateManager.lock()->inletChanged();
	}

	void BasicInletIO::setData( std::shared_ptr< const CustomType > data )
	{
		TimestampedData timestamped( data, mEngineImpl->getElapsedTime() );
		mBuffer->setData( timestamped );
		if ( canTriggerUpdates() ) mUpdateManager.lock()->inletChanged();
	}

	void BasicInletIO::synchronize()
	{
		std::shared_ptr< const CustomType > newData = mBuffer->getData().value;
		mInlet->update( newData );
		mLastData = mBuffer->getData();
		mBuffer->clearData();
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

	MultiInletIO::MultiInletIO( EngineImpl *engine, std::shared_ptr< InstanceId > id, AbstractUberBlock *owner, std::shared_ptr< AbstractUpdateManager > updateMgr, std::shared_ptr< const IOMetadata > meta ) :
		AbstractInletIO(),
		mInlet( new MultiInlet( this ) ),
		mEngineImpl( engine ),
		mUpdateManager( updateMgr ),
		mOwningBlock( owner ),
		mMetadata( meta ),
		mIdentifier( id )
	{
	}

	std::string const& MultiInletIO::getFullHumanReadableName() const
	{
		return mIdentifier->getFullHumanReadableName();
	}

	std::string const& MultiInletIO::getHumanReadableName() const
	{
		return mIdentifier->getHumanReadableName();
	}

	//std::string const& MultiInletIO::getCode() const
	//{
	//	return mIdentifier.getCode();
	//}

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
		return std::shared_ptr< AbstractInletIO >();
	}

	std::shared_ptr< AbstractInlet > MultiInletIO::getInlet()
	{
		return mInlet;
	}

	std::shared_ptr< const AbstractInlet > MultiInletIO::getInlet() const
	{
		return mInlet;
	}

	std::shared_ptr< const IOMetadata > MultiInletIO::getMetadata() const
	{
		return mMetadata;
	}

	bool MultiInletIO::belongsToBlock( AbstractUberBlock const* block ) const
	{
		return ( mOwningBlock == block );
	}

	void MultiInletIO::removeSubInlet( std::shared_ptr< AbstractInletIO > io )
	{
		assert( NULL );
	}

	void MultiInletIO::synchronize()
	{
		assert( NULL );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	OutletIO::OutletIO( EngineImpl *engine, std::shared_ptr< InstanceId > id, AbstractUberBlock *owner, std::shared_ptr< const IOMetadata > meta ) :
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mOutlet( new Outlet( this ) ),
		mBuffer( new OutletBuffer( this ) ),
		mMetadata( meta ),
		mAppEvent( new CallbackEvent< std::shared_ptr< const CustomType > >() ),
		mInletEvent( new CallbackEvent< TimestampedData const& >() ),
		mIdentifier( id )
	{
	}

	OutletIO::~OutletIO()
	{
		delete mBuffer;
		delete mAppEvent;
		delete mInletEvent;
	}

	std::string const& OutletIO::getFullHumanReadableName() const
	{
		return mIdentifier->getFullHumanReadableName();
	}

	std::string const& OutletIO::getHumanReadableName() const
	{
		return mIdentifier->getHumanReadableName();
	}

	//std::string const& OutletIO::getCode() const
	//{
	//	return mIdentifier.getCode();
	//}

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

	std::shared_ptr< const IOMetadata > OutletIO::getMetadata() const
	{
		return mMetadata;
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
		std::shared_ptr< CustomType > newData( new CustomType( *( mMetadata->initializer.get() ) ) ) ;
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

}