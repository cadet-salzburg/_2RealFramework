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

#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "engine/_2RealParameterData.h"

using std::string;

namespace _2Real
{

	InletIO::InletIO( AbstractUberBlock &owner, ParameterData const& data ) :
		Handleable< app::InletHandle >( *this ),
		m_Inlet( new Inlet( owner, data.getName(), data.getLongTypename(), data.getTypename() ) ),
		m_Buffer( new InletBuffer( data.getDefaultValue() ) )
	{
	}

	OutletIO::OutletIO( AbstractUberBlock &owner, ParameterData const& data ) :
		Handleable< app::OutletHandle >( *this ),
		m_Outlet( new Outlet( owner, data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() ) ),
		m_AppEvent( new CallbackEvent< app::AppData const& >() ),
		m_InletEvent( new CallbackEvent< TimestampedData const& >() )
	{
	}

	InletIO::~InletIO()
	{
		delete m_Inlet;
		delete m_Buffer;
	}

	OutletIO::~OutletIO()
	{
		delete m_Outlet;
		delete m_AppEvent;
		delete m_InletEvent;
	}

	AbstractIOManager::AbstractIOManager( AbstractUberBlock &owner ) :
		m_Owner( owner )
	{
	}

	AbstractIOManager::~AbstractIOManager()
	{
	}

	string const& AbstractIOManager::getName() const
	{
		return m_Owner.getName();
	}

	unsigned int AbstractIOManager::getId() const
	{
		return m_Owner.getId();
	}

}