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
#include "_2RealInletBuffer.h"
#include "_2RealOutlet.h"
#include "_2RealParameter.h"
#include "_2RealParameterData.h"

#include <iostream>

namespace _2Real
{

	// some free functions

	Inlet * createInletFromParameterData( AbstractUberBlock &owner, ParameterData const& data )
	{
		Inlet *inlet = new Inlet( owner, data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		return inlet;
	}

	Outlet * createOutletFromParameterData( AbstractUberBlock &owner, ParameterData const& data )
	{
		Outlet *outlet = new Outlet( owner, data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		return outlet;
	}

	Parameter * createParameterFromParameterData( AbstractUberBlock &owner, ParameterData const& data )
	{
		Parameter *parameter = new Parameter( owner, data.getName(), data.getLongTypename(), data.getTypename() );
		parameter->setData( TimestampedData( data.getDefaultValue(), 0 ) );
		return parameter;
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

	unsigned int AbstractIOManager::getId() const
	{
		return m_Owner.getId();
	}

}