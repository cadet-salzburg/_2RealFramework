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

namespace _2Real
{

	class Handle
	{

	public:

		Handle() :
			m_Obj( nullptr )
		{
		}

		Handle( void *obj ) :
			m_Obj( obj )
		{
		}

		virtual ~Handle()
		{
		}

		bool isValid() const
		{
			return m_Obj != nullptr;
		}

		void invalidate()
		{
			m_Obj = nullptr;
		}

		bool operator==( Handle const& other ) const
		{
			return m_Obj == other.m_Obj;
		}

		bool operator!=( Handle const& other ) const
		{
			return m_Obj != other.m_Obj;
		}

		bool operator<( Handle const& other ) const
		{
			return m_Obj < other.m_Obj;
		}

		bool operator<=( Handle const& other ) const
		{
			return m_Obj <= other.m_Obj;
		}

		bool operator>( Handle const& other ) const
		{
			return m_Obj > other.m_Obj;
		}

		bool operator>=( Handle const& other ) const
		{
			return m_Obj >= other.m_Obj;
		}

	private:

		void *m_Obj;

	};

	class AbstractHandleAble
	{
	public:
		virtual ~AbstractHandleAble()
		{
			for ( HandleIterator it = m_Handles.begin(); it != m_Handles.end(); ++it )
			{
				( *it )->invalidate();
			}
		}
		void registerHandle( Handle &handle )
		{
			m_Handles.insert( &handle );
		}
		void unregisterHandle( Handle &handle )
		{
			HandleIterator it = m_Handles.find( &handle );
			if ( it != m_Handles.end() )
			{
				m_Handles.erase( it );
			}
		}
	private:
		typedef std::set< Handle * >				Handles;
		typedef std::set< Handle * >::iterator		HandleIterator;

		Handles		m_Handles;
	};

	template< typename THandle >
	class HandleAble : public virtual AbstractHandleAble
	{

	public:

		template< typename TObj >
		HandleAble( TObj &obj ) : m_Handle( new THandle( obj ) ) {}
		virtual ~HandleAble() { delete m_Handle; }
		THandle& getHandle() const { return *m_Handle; }

	private:

		THandle		*m_Handle;

	};

}