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

#include "engine/_2RealLink.h"
#include "engine/_2RealDataSource_I.h"
#include "engine/_2RealDataSink_I.h"
#include "common/_2RealException.h"

namespace _2Real
{
	std::shared_ptr< Link > Link::createLink( std::shared_ptr< DataSink_I > sink, std::shared_ptr< DataSource_I > source )
	{
		std::shared_ptr< Link > result( new Link );

		// link listens to source removal & source data
		result->linkTo( source );

		// link listens to sink removal
		result->mSinkRemovedConnection = sink->registerToRemoved( std::bind( &Link::sinkRemoved, result.get() ) );

		// sink listens to link-as-source removal & link data
		sink->linkTo( result );

		// no-one listens to link-as-sink removal

		return result;
	}

	void Link::linkTo( std::shared_ptr< DataSource_I > source )
	{
		mDataConnection = source->registerToUpdate( std::bind( &Link::receiveData, this, std::placeholders::_1 ) );
		mSourceRemovedConnection = source->registerToRemoved( std::bind( &Link::sourceRemoved, this ) );
	}

	void Link::receiveData( std::shared_ptr< const DataItem > data )
	{
		mData( data );
	}

	void Link::sinkRemoved()
	{
		// sink: has removed itself from link-as-source
		// does not need link-as-source removal signal
		mSinkRemovedConnection.disconnect();

		// source: listens to nothing, anyway
		// link-as-sink removal does not matter
		mDataConnection.disconnect();
		mSourceRemovedConnection.disconnect();

		// collection: listens to link removal
		// this causes the collection to drop the shared ptr
		mDestroyed( shared_from_this() );
	}

	void Link::sourceRemoved()
	{
		// source: listens to nothing, anyway
		// link-as-sink removal does not matter
		mDataConnection.disconnect();
		mSourceRemovedConnection.disconnect();

		// sink: still listens to link-as-source
		mSourceRemoved( std::static_pointer_cast< DataSource_I >( shared_from_this() ) );
		mSinkRemovedConnection.disconnect();

		// collection: listens to link removal
		// this causes the collection to drop the shared ptr
		mDestroyed( shared_from_this() );
	}

	void Link::destroy()
	{
		// source: listens to nothing, anyway
		// link-as-sink removal does not matter
		mDataConnection.disconnect();
		mSourceRemovedConnection.disconnect();

		// sink: still listens to link-as-source
		// this will be stopped now
		mSourceRemoved( std::static_pointer_cast< DataSource_I >( shared_from_this() ) );
		mSinkRemovedConnection.disconnect();

		// collection: listens to link removal
		// this causes the collection to drop the shared ptr
		mDestroyed( shared_from_this() );
	}

	boost::signals2::connection Link::registerToUpdate( boost::signals2::signal< void( std::shared_ptr< const DataItem > ) >::slot_type listener ) const
	{
		return mData.connect( listener );
	}

	boost::signals2::connection Link::registerToDestroyed( boost::signals2::signal< void( std::shared_ptr< const Link > ) >::slot_type listener ) const
	{
		return mDestroyed.connect( listener );
	}

	boost::signals2::connection Link::registerToRemoved( boost::signals2::signal< void( std::shared_ptr< const DataSource_I > ) >::slot_type listener ) const
	{
		return mSourceRemoved.connect( listener );
	}

	boost::signals2::connection Link::registerToRemoved( boost::signals2::signal< void( std::shared_ptr< const DataSink_I > ) >::slot_type listener ) const
	{
		return mSinkRemoved.connect( listener );
	}
}
