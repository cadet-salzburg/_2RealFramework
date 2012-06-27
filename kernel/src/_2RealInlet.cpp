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

using std::string;

namespace _2Real
{

	Inlet::Inlet( string const& name, string const& longTypename, string const& type, EngineData const& defaultValue ) :
		Parameter( name, longTypename, type ),
		m_Buffer( defaultValue ),
		m_CurrentData( EngineData(), 0 ),
		m_LastData( EngineData(), 0 )
	{
	}

	EngineData const& Inlet::getCurrentValue() const
	{
		return m_CurrentData.getData();
	}

	void Inlet::updateCurrentValue()
	{
		m_CurrentData = m_Buffer.getCurrentData();
	}

	void Inlet::updateDataBuffer()
	{
		m_Buffer.updateDataBuffer();
	}

	void Inlet::setDefaultValue( TimestampedData const& defaultValue )
	{
		m_Buffer.setDefaultData( defaultValue );
	}

	void Inlet::disableTriggering( TimestampedData const& data )
	{
		m_Buffer.disableTriggering( data );
	}

	void Inlet::registerUpdateTrigger( AbstractInletBasedTrigger &trigger )
	{
		m_Buffer.registerUpdateTrigger( trigger );
	}

	void Inlet::unregisterUpdateTrigger( AbstractInletBasedTrigger &trigger )
	{
		m_Buffer.unregisterUpdateTrigger( trigger );
	}

}