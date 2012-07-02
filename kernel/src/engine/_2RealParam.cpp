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

#include "engine/_2RealParameter.h"

using std::string;

namespace _2Real
{

	Param::Param( AbstractUberBlock &owningBlock, string const& name, string const& longTypename, string const& typeName ) :
		m_Owner( owningBlock ),
		m_Name( name ),
		m_LongTypename( longTypename ),
		m_Typename( typeName )
	{
	}

	std::string const& Param::getName() const
	{
		return m_Name;
	}

	std::string const& Param::getTypename() const
	{
		return m_Typename;
	}

	std::string const& Param::getLongTypename() const
	{
		return m_LongTypename;
	}

	AbstractUberBlock& Param::getOwningUberBlock()
	{
		return m_Owner;
	}

}