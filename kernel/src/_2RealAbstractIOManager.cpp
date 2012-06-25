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

#include "_2RealAbstractIOManager.h"
#include "_2RealAbstractUberBlock.h"
#include "_2RealInlet.h"
#include "_2RealOutlet.h"
#include "_2RealSetupParameter.h"
#include "_2RealParameterData.h"

namespace _2Real
{

	// some free functions

	Inlet & createInletFromParameterData( ParameterData const& data )
	{
		Inlet *inlet = new Inlet( data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		return *inlet;
	}

	Outlet & createOutletFromParameterData( ParameterData const& data )
	{
		Outlet *outlet = new Outlet( data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		return *outlet;
	}

	SetupParameter & createSetupParameterFromParameterData( ParameterData const& data )
	{
		SetupParameter *parameter = new SetupParameter( data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		return *parameter;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	AbstractIOManager::AbstractIOManager(AbstractUberBlock &owner) :
		m_Owner(owner)
	{
	}

	AbstractIOManager::~AbstractIOManager()
	{
	}

	std::string const& AbstractIOManager::getName() const
	{
		return m_Owner.getName();
	}

	const unsigned int AbstractIOManager::getId() const
	{
		return m_Owner.getId();
	}

}