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

#include "helpers/_2RealCallback_T.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	template< typename TArg >
	class Event_T
	{

	public:

		template< typename TArg >
		struct Compare_T
		{
			bool operator()( std::shared_ptr< AbstractCallback_T< TArg > > a, std::shared_ptr< AbstractCallback_T< TArg > > b )
			{
#ifdef _DEBUG
				assert( a.get() );
				assert( b.get() );
#endif
				return ( *a.get() < *b.get() );
			}
		};

		typedef std::set< std::shared_ptr< AbstractCallback_T< TArg > >, Compare_T< TArg > > EventListeners;

		Event_T() {}
		~Event_T() { clear(); }

		void clear()
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			mListeners.clear();
		}

		void addListener( std::shared_ptr< AbstractCallback_T< TArg > > listener )
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			EventListeners::iterator it = mListeners.find( listener );
			if ( it == mListeners.end() )
				mListeners.insert( listener );
		}

		void removeListener( std::shared_ptr< AbstractCallback_T< TArg > > listener )
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			EventListeners::iterator it = mListeners.find( listener );
			if ( it != mListeners.end() )
				mListeners.erase( it );
		}

		void notify( TArg arg ) const
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			for ( auto it : mListeners )
				( *it ).invoke( arg );
		}

	private:

		Event_T( Event_T const& other );
		Event_T& operator=( Event_T const& other );

		mutable std::mutex		mListenerAccess;
		EventListeners			mListeners;

	};

	template<>
	class Event_T< void >
	{

	public:

		struct Compare
		{
			bool operator()( std::shared_ptr< AbstractCallback_T< void > > a, std::shared_ptr< AbstractCallback_T< void > > b )
			{
#ifdef _DEBUG
				assert( a.get() );
				assert( b.get() );
#endif
				return ( *a.get() < *b.get() );
			}
		};

		typedef std::set< std::shared_ptr< AbstractCallback_T< void > >, Compare > EventListeners;

		Event_T() {}
		~Event_T() { clear(); }

		void clear()
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			mListeners.clear();
		}

		void addListener( std::shared_ptr< AbstractCallback_T< void > > listener )
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			EventListeners::iterator it = mListeners.find( listener );
			if ( it == mListeners.end() )
				mListeners.insert( listener );
		}

		void removeListener( std::shared_ptr< AbstractCallback_T< void > > listener )
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			EventListeners::iterator it = mListeners.find( listener );
			if ( it != mListeners.end() )
				mListeners.erase( it );
		}

		void notify() const
		{
			std::lock_guard< std::mutex > lock( mListenerAccess );
			for ( auto it : mListeners )
				( *it ).invoke();
		}

	private:

		Event_T( Event_T const& other );
		Event_T& operator=( Event_T const& other );

		mutable std::mutex		mListenerAccess;
		EventListeners			mListeners;

	};
}
