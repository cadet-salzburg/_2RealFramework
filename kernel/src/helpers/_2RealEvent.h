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

#include "helpers/_2RealPoco.h"
#include "helpers/_2RealCallback.h"

#include <set>
#include <iostream>

namespace _2Real
{
	template< typename TArg >
	class CallbackEvent
	{

	public:

		typedef std::set< AbstractCallback< TArg > *, AbstractCallbackCompare< TArg > >								Callbacks;
		typedef typename std::set< AbstractCallback< TArg > *, AbstractCallbackCompare< TArg > >::iterator			CallbackIterator;
		typedef typename std::set< AbstractCallback< TArg > *, AbstractCallbackCompare< TArg > >::const_iterator	CallbackConstIterator;

		CallbackEvent() {}
		~CallbackEvent() { clear(); }

		void clear()
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			for ( CallbackIterator cbIter = m_Callbacks.begin(); cbIter != m_Callbacks.end(); ++cbIter ) delete *cbIter;
			m_Callbacks.clear();
		}

		void addListener( AbstractCallback< TArg > &callback )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			CallbackIterator cbIter = m_Callbacks.find( &callback );
			if ( cbIter == m_Callbacks.end() )
			{
				m_Callbacks.insert( &callback );
			}
			else delete &callback;
		}

		void removeListener( AbstractCallback< TArg > &callback )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			CallbackIterator cbIter = m_Callbacks.find( &callback );
			if ( cbIter != m_Callbacks.end() )
			{
				delete *cbIter;
				m_Callbacks.erase( cbIter );
			}
			delete &callback;
		}

		void notify( TArg &arg ) const
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			for ( CallbackIterator cbIter = m_Callbacks.begin(); cbIter != m_Callbacks.end(); ++cbIter )
			{
				( *cbIter )->invoke( arg );
			}
		}

	private:

		mutable Poco::FastMutex		m_Access;
		Callbacks					m_Callbacks;

	};
}
