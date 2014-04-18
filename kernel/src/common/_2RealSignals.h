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

#pragma once

#include "common/_2RealBoost.h"
#pragma warning(disable: 4512)		// blah blah blah
#pragma warning(disable: 4996)		// msvc sure is paranoid :/
#include <boost/signals2.hpp>

namespace _2Real
{
	//class Signal_I
	//{
	//public:
	//	virtual ~Signal_I() = default;
	//	virtual void disconnect( const uint64_t ) = 0;
	//};

	//class Connection
	//{

	//public:

	//	Connection( std::shared_ptr< Signal_I > signal, const uint64_t slot ) : mSignal( signal ), mSlot( slot )
	//	{
	//	}

	//	~Connection()
	//	{
	//		reset();
	//	}

	//	void reset()
	//	{
	//		std::shared_ptr< Signal_I > sig = mSignal.lock();
	//		sig->disconnect( mSlot );
	//	}

	//private:

	//	std::weak_ptr< Signal_I >	mSignal;
	//	uint64_t					mSlot;

	//};

	//template< typename TLockable, typename TReturn, typename... TArgs >
	//class Signal_T : public Signal_I, public std::enable_shared_from_this< Signal_T< TLockable, TReturn, TArgs... > >
	//{

	//	typedef std::map< uint64_t, std::function< TReturn( TArgs... ) > > Connections;

	//public:

	//	void clear() 
	//	{
	//		std::lock_guard< TLockable > lock( mLock );

	//		mConnections.clear();
	//	}

	//	Connection connect( std::function< TReturn( TArgs... ) > listener )
	//	{
	//		std::lock_guard< TLockable > lock( mLock );

	//		uint64_t slot = ++sConnectionCounter;
	//		Connection connection( shared_from_this(), slot );
	//		mConnections.insert( std::make_pair( slot, listener ) );
	//		return connection;
	//	}

	//	void disconnect( const uint64_t slot )
	//	{
	//		std::lock_guard< TLockable > lock( mLock );

	//		auto it = mConnections.find( slot );
	//		if ( it != mConnections.end() )
	//			mConnections.erase( it );

	//		// fail silent otherwise
	//	}

	//	TReturn notify( TArgs... args ) const
	//	{
	//		std::lock_guard< TLockable > lock( mLock );
	//
	//		for ( auto it : mConnections )
	//		{
	//			std::function< TReturn( TArgs... ) >func = it.second;
	//			func()( ...args );
	//		}
	//	}

	//private:

	//	static uint64_t sConnectionCounter;

	//	mutable TLockable		mLock;
	//	Connections				mConnections;

	//};

	//template< typename TLockable, typename TReturn, typename... TArgs >
	//uint64_t Signal_T< TLockable, TReturn, TArgs... >::sConnectionCounter = 0;

}
