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

#include "_2RealAbstractServiceVariable.h"

#include "_2RealData.h"

namespace _2Real
{
	AbstractServiceVariable::AbstractServiceVariable(std::string const& _name) : m_OriginalName(_name), m_FrameworkName(Variable())
	{
	}

	std::string const& AbstractServiceVariable::originalName() const
	{
		return m_OriginalName;
	}

	Variable const& AbstractServiceVariable::frameworkName() const
	{
		return m_FrameworkName;
	}

	void AbstractServiceVariable::setFrameworkName(Variable const& _name)
	{
		m_FrameworkName = _name;
	}

	const bool AbstractServiceVariable::getFrom(VariableData const& _data, Poco::Any &_any)
	{
		AnyPtr anyPtr = _data.getAny(m_FrameworkName);

		if (anyPtr.isNull())
		{
			return false;
		}

		_any = *anyPtr.get();
		return true;
	}

	const bool AbstractServiceVariable::insertInto(VariableData &_data, Poco::Any &_any) const
	{
		AnyPtr anyPtr = AnyPtr(new Poco::Any(_any));
		_data.insertAny(m_FrameworkName, anyPtr);
		return true;
	}
}