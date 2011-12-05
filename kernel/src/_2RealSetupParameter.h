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

#pragma once

#include "_2RealParameter.h"
#include "_2RealEngineData.h"

namespace _2Real
{

	/**
	*	class for setup parameters: service-setup & plugin-setup
	*/

	class SetupParameter : public Parameter
	{

	public:

		SetupParameter(std::string const& name, std::string const& type, std::string const& keyword);
		virtual ~SetupParameter();

		void setData(EngineData const& data);
		EngineData const& getData() const;

	protected:

		EngineData				m_Data;

	};

	inline SetupParameter::SetupParameter(std::string const& name, std::string const& type, std::string const& keyword) :
		Parameter(name, type, keyword)
	{
	}

	inline SetupParameter::~SetupParameter()
	{
	}

	inline void SetupParameter::setData(EngineData const& data)
	{
		m_Data = data;
		m_IsInitialized = true;
	}

	inline EngineData const& SetupParameter::getData() const
	{
		return m_Data;
	}

}