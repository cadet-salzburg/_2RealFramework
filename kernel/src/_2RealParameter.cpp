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

#include "_2RealParameter.h"
#include "_2RealException.h"

namespace _2Real
{

	Parameter::Parameter(Id *const _id, std::string const& _type) :
		Entity(_id),
		m_bIsInitialized(false),
		m_Typename(_type)
	{
	}

	Parameter::Parameter(Parameter const& _src) : Entity(_src)
	{
		throw Exception("attempted to copy entity");
	}

	Parameter& Parameter::operator=(Parameter const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	Parameter::~Parameter()
	{
	}

	bool const& Parameter::isInitialized() const
	{
		return m_bIsInitialized;
	}

	std::string const& Parameter::datatype() const
	{
		return m_Typename;
	}

}