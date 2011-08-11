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

#include "_2RealConfigData.h"
#include "_2RealAbstractServicevariable.h"

namespace _2Real
{
	ConfigurationData::ConfigurationData(std::string const& _name, ConfigurationData *const _father) :
		Metadata(_name, _father)
	{
	}

	ConfigurationData::ConfigurationData(ConfigurationData const& _src) :
		Metadata(_src), m_SetupValues(_src.m_SetupValues)
	{
	}

	ConfigurationData& ConfigurationData::operator=(ConfigurationData const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		Metadata::operator=(_src);

		m_SetupValues = _src.m_SetupValues;

		return *this;
	}

	ConfigurationData::~ConfigurationData()
	{
	}

	const bool ConfigurationData::configureValue(AbstractServiceVariable *const _var) const
	{
		return true;
	}

	const bool ConfigurationData::configureName(AbstractServiceVariable *const _var) const
	{
		return true;
	}

	const bool addToValues(AbstractServiceVariable *_var)
	{
		//_var->insertInto(m_SetupValues);
		return true;
	}

	const bool addToVariables(AbstractServiceVariable *_var)
	{
		//Metadata::insert();
		return true;
	}

}