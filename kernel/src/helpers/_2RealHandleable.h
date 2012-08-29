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

#include <set>
#include <iostream>

namespace _2Real
{
	template< typename TObj, typename THandle >
	class Handleable
	{

	private:

		typedef std::set< THandle * >							Handles;
		typedef typename std::set< THandle * >::iterator		HandleIterator;
		typedef typename std::set< THandle * >::const_iterator	HandleConstIterator;

		TObj			&m_Obj;
		mutable THandle	*m_Handle;
		Handles			m_Handles;

	protected:

		Handleable( TObj &obj ) :
			m_Obj( obj ),
			m_Handle( nullptr )
		{
		}

		virtual ~Handleable()
		{
			delete m_Handle;
			for ( HandleIterator it = m_Handles.begin(); it != m_Handles.end(); ++it )
			{
				( *it )->invalidate();
			}
		}

		THandle& getHandle() const
		{
			if ( m_Handle == nullptr )
			{
				m_Handle = new THandle( m_Obj );
			}

			return *m_Handle;
		}

		void registerHandle( THandle &handle )
		{
			m_Handles.insert( &handle );
		}

		void unregisterHandle( THandle &handle )
		{
			HandleIterator it = m_Handles.find( &handle );
			if ( it != m_Handles.end() )
			{
				m_Handles.erase( it );
			}
		}
	};
}
