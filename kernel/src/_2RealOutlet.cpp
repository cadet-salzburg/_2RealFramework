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

#include "_2RealOutlet.h"
#include "_2RealInlet.h"
#include "_2RealInletBuffer.h"
#include "_2RealException.h"
#include "_2RealFunctionBlock.h"
#include "_2RealParameterData.h"
#include "_2RealEngineImpl.h"

#include "Poco/Delegate.h"

using std::string;
using std::find;
using std::ostringstream;

namespace _2Real
{

	Outlet::Outlet( string const& name, string const& longTypename, string const& typeName, EngineData const& emptyData ) :
		Parameter( name, longTypename, typeName ),
		m_Engine( EngineImpl::instance() ),
		m_CurrentDataItem( emptyData, 0 ),
		m_LastDataItem( emptyData, 0 ),
		m_DiscardCurrent( false )
	{
	}

	void Outlet::update()
	{
		Poco::FastMutex::ScopedLock lock( m_Access );

		if ( !m_DiscardCurrent )
		{
			// last data will hold the data of the last update cycle
			m_LastDataItem = TimestampedData( m_CurrentDataItem.getData(), m_Engine.getElapsedTime() );

			// copy last data back into the outlet's current data
			m_CurrentDataItem.cloneData( m_LastDataItem );

			// inlets get the datat that was really written, not a copy
			m_InletEvent.notify( this, m_LastDataItem );

			// create a copy for the application ( might not be necessary depending on listeners? )
			EngineData copy;
			copy.cloneFrom( m_LastDataItem.getData() );
			m_LastOutputData = OutputData( copy, Parameter::getTypename(), Parameter::getName() );
		}
	}

	EngineData const& Outlet::getLastData() const
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		return m_LastDataItem.getData();
	}

	OutputData const& Outlet::getLastOutputData() const
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		return m_LastOutputData;
	}

	EngineData & Outlet::getCurrentData()
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		return m_CurrentDataItem.getData();
	}

	void Outlet::discardCurrentData()
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		m_DiscardCurrent = true;
	}

	void Outlet::addInletListener( InletBuffer &buffer )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		m_InletEvent += Poco::delegate( &buffer, &InletBuffer::receiveData );
		if ( !m_LastDataItem.isEmpty() )
		{
			buffer.receiveData( m_LastDataItem );
		}
	}

	void Outlet::removeInletListener( InletBuffer &buffer )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		m_InletEvent -= Poco::delegate( &buffer, &InletBuffer::receiveData );
	}

}