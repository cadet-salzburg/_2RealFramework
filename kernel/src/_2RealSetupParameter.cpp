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

#include "_2RealSetupParameter.h"
#include "_2RealException.h"

#include <iostream>

namespace _2Real
{

	SetupParameter::SetupParameter(Id *const _id, std::string const& _type) :
		Parameter(_id, _type)
	{
	}

	SetupParameter::SetupParameter(SetupParameter const& _src) : Parameter(_src)
	{
		throw Exception("attempted to copy entity");
	}

	SetupParameter& SetupParameter::operator=(SetupParameter const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	SetupParameter::~SetupParameter()
	{
	}

	void SetupParameter::setValue(Poco::Any const& _any)
	{
		m_Value = _any;
	}

	Poco::Any const& SetupParameter::value() const
	{
		return m_Value;
	}

}