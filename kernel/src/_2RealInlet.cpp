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
		virtual bool insertData( std::pair< long, EngineData > &data, DataBuffer &buffer ) = 0;
		virtual pair< long, EngineData > getData( DataBuffer &buffer ) = 0;

	};

	class AlwaysInsert : public BufferPolicy
	{

	public:

		AlwaysInsert( const unsigned int max ) : BufferPolicy(), m_Max( max ) {}
		void setMaxBufferSize( const unsigned int max ) { m_Max = max; }

		bool insertData( pair< long, EngineData > &data, DataBuffer &buffer )
		{
			if ( buffer.size() >= m_Max )
			{
				DataBuffer::iterator it = buffer.end();
				it--;
				buffer.erase( it );
			}

			buffer.insert( data );
			return true;
		}

		pair< long, EngineData > getData( DataBuffer &buffer )
		{
			DataBuffer::iterator it = buffer.begin();
			return *it;
		}

	private:

		unsigned int		m_Max;

	};

	//Inlet::Inlet( ParameterData const& metadata ) :
	//	Parameter( metadata ),
	//	m_BufferPolicy( new AlwaysInsert( 10 ) ),
	//	m_LastTimestamp( 0 ),
	//	m_HasFixedData( false )
	//{
	//	m_DefaultData = std::make_pair( m_LastTimestamp, metadata.getDefaultValue() );	//default data has 0 as timestamp
	//	m_BufferPolicy->insertData( m_DefaultData, m_ReceivedTable );
	//	m_CurrentData = m_DefaultData;
	//}

	Inlet::Inlet( string const& name, string const& longTypename, string const& type, EngineData const& defaultValue ) :
		Parameter( name, longTypename, type ),
		m_BufferPolicy( new AlwaysInsert( 10 ) ),
		m_HasFixedData( false ),
		m_LastTimestamp( 0 ),
		m_DefaultData( make_pair( 0, defaultValue ) ),
		m_CurrentData( make_pair( 0, defaultValue ) )
	{
		m_BufferPolicy->insertData( m_DefaultData, m_ReceivedTable );
	}

	Inlet::~Inlet()
	{
		delete m_BufferPolicy;
	}

	void Inlet::updateCurrentData()
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );
		m_CurrentData = m_BufferPolicy->getData( m_ReceivedTable );
	}

	void Inlet::resetData()
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );
		if ( m_HasFixedData )
		{
			m_BufferPolicy->insertData( m_FixedData, m_ReceivedTable );

			std::pair< long, long > times = std::make_pair< long, long >( m_LastTimestamp, m_FixedData.first );
			m_DataReceived.notify( this, times );
		}
		else
		{
			m_BufferPolicy->insertData( m_DefaultData, m_ReceivedTable );

			std::pair< long, long > times = std::make_pair< long, long >( m_LastTimestamp, m_DefaultData.first );
			m_DataReceived.notify( this, times );
		}
	}

	void Inlet::setFixedData( TimestampedData const& data )
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );

		clearLinks();

		m_HasFixedData = true;
		m_LastTimestamp = data.getTimestamp();
		m_FixedData = std::make_pair( m_LastTimestamp, data.getData() );

		//the only point where i directly access the data buffer
		m_ReceivedTable.clear();
		m_ReceivedTable.insert( m_FixedData );

		std::pair< long, long > times = std::make_pair< long, long >( 0, m_LastTimestamp ); // on first receiving the fixed value, 'new' is always fulfilled
		m_DataReceived.notify( this, times );
	}

	void Inlet::receiveData( TimestampedData &data )
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );

		long time = data.getTimestamp();
		if ( m_BufferPolicy->insertData( std::make_pair( time, data.getData() ), m_ReceivedTable ) )
		{
			std::pair< long, long > times = std::make_pair< long, long >( m_LastTimestamp, time );
			m_DataReceived.notify( this, times );
		}
	}

	EngineData const& Inlet::getCurrentData() const
	{
		Poco::FastMutex::ScopedLock lock( m_DataAccess );
		return m_CurrentData.second;
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