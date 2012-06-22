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

#include "_2RealInlet.h"
#include "_2RealOutlet.h"
#include "_2RealException.h"
#include "_2RealTimestampedData.h"
#include "_2RealParameterData.h"
#include "_2RealAbstractStateManager.h"

#include <iostream>

#include "Poco/Delegate.h"

using std::map;
using std::string;
using std::pair;
using std::make_pair;

namespace _2Real
{

	class BufferPolicy
	{

	public:

		virtual ~BufferPolicy() {}
		virtual bool receiveData( TimestampedData &data, DataBuffer &buffer ) = 0;
		virtual TimestampedData const& getData( DataBuffer &buffer ) = 0;
		virtual void setDefault( TimestampedData const& data ) = 0;

	};

	class AlwaysInsert : public BufferPolicy
	{

	public:

		AlwaysInsert( const unsigned int max ) : BufferPolicy(), m_Max( max ) {}
		void setMaxBufferSize( const unsigned int max ) { m_Max = max; }

		void setDefault( TimestampedData const& data )
		{
			m_Default = data;
		}

		bool receiveData( TimestampedData &data, DataBuffer &buffer )
		{
			if ( buffer.size() >= m_Max )
			{
				// deletes oldest
				DataBuffer::iterator it = buffer.end();
				it--;
				buffer.erase( it );
			}

			buffer.insert( data );
			return true;
		}

		// this is actually a bit messy
		TimestampedData const& getData( DataBuffer &buffer )
		{
			if ( buffer.empty() )
			{
				return m_Default;
			}
			else
			{
				DataBuffer::iterator it = buffer.begin();
				return *it;
			}
		}

	private:

		TimestampedData		m_Default;
		unsigned int		m_Max;

	};

	Inlet::Inlet( string const& name, string const& longTypename, string const& type, EngineData const& defaultValue ) :
		Parameter( name, longTypename, type ),
		m_BufferPolicy( new AlwaysInsert( 10 ) ),
		m_DefaultValue( defaultValue, 0 )
	{
		m_BufferPolicy->setDefault( TimestampedData( defaultValue, 0 ) );
	}

	Inlet::~Inlet()
	{
		delete m_BufferPolicy;
	}

	void Inlet::updateCurrentValue()
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );

		// returns default or last item
		m_CurrentValue = m_BufferPolicy->getData( m_ReceivedDataItems );
	}

	// ok, this is seriously a mess
	void Inlet::resetData()
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );

		std::pair< long, long > times = std::make_pair< long, long >( m_CurrentValue.getTimestamp(), m_DefaultValue.getTimestamp() );
		m_DataReceived.notify( this, times );
	}

	void Inlet::setToValue( TimestampedData const& data )
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );

		m_DefaultValue = data;
		m_BufferPolicy->setDefault( data );
	}

	void Inlet::receiveData( TimestampedData &data )
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );

		long time = data.getTimestamp();
		if ( m_BufferPolicy->receiveData( data, m_ReceivedDataItems) )
		{
			std::pair< long, long > times = std::make_pair< long, long >( m_CurrentValue.getTimestamp(), time );
			m_DataReceived.notify( this, times );
		}
	}

	EngineData const& Inlet::getCurrentValue() const
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );
		return m_CurrentValue.getData();
	}

	void Inlet::linkWith(Outlet &outlet)
	{
		Poco::FastMutex::ScopedLock lock( m_OutletsAccess );

		std::list< Outlet * >::iterator it = std::find< std::list< Outlet * >::iterator, Outlet * >(m_LinkedOutlets.begin(), m_LinkedOutlets.end(), &outlet);
		if ( it == m_LinkedOutlets.end() )
		{
			m_LinkedOutlets.push_back(&outlet);
			outlet.addListener(*this);
		}
	}

	void Inlet::breakLink( Outlet &outlet )
	{
		unlink( outlet );
		outlet.removeListener(*this);
	}

	void Inlet::unlink( Outlet &outlet )
	{
		Poco::FastMutex::ScopedLock lock( m_OutletsAccess );

		std::list< Outlet * >::iterator it = std::find< std::list< Outlet * >::iterator, Outlet * >(m_LinkedOutlets.begin(), m_LinkedOutlets.end(), &outlet);
		if (it != m_LinkedOutlets.end())
		{
			m_LinkedOutlets.erase(it);
		}
	}

	void Inlet::clearLinks()
	{
		Poco::FastMutex::ScopedLock lock( m_OutletsAccess );

		for ( std::list< Outlet * >::iterator it = m_LinkedOutlets.begin(); it != m_LinkedOutlets.end(); ++it )
		{
			(*it)->removeListener( *this );
		}

		m_LinkedOutlets.clear();
	}

	void Inlet::registerToDataReceived( AbstractStateManager &triggers )
	{
		m_DataReceived += Poco::delegate( &triggers, &AbstractStateManager::tryTriggerInlet );
	}

	void Inlet::unregisterFromDataReceived( AbstractStateManager &triggers )
	{
		m_DataReceived -= Poco::delegate( &triggers, &AbstractStateManager::tryTriggerInlet );
	}

}