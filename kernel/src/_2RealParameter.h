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

#include <string>

namespace _2Real
{

	class Parameter
	{

	public:

		Parameter(std::string const& name, std::string const& type, std::string const& keyword);
		virtual ~Parameter();

		bool					isInitialized() const;
		std::string const&		keyword() const;
		std::string const&		datatype() const;
		std::string const&		name() const;

	protected:

		bool					m_IsInitialized;
		const std::string		m_Datatype;
		const std::string		m_Keyword;
		const std::string		m_Name;

	};

	inline Parameter::Parameter(std::string const& name, std::string const& type, std::string const& keyword) :
		m_IsInitialized(false),
		m_Name(name),
		m_Datatype(type),
		m_Keyword(keyword)
	{
	}

	inline Parameter::~Parameter()
	{
	}

	inline bool Parameter::isInitialized() const
	{
		return m_IsInitialized;
	}

	inline std::string const& Parameter::name() const
	{
		return m_Name;
	}

	inline std::string const& Parameter::keyword() const
	{
		return m_Keyword;
	}

	inline std::string const& Parameter::datatype() const
	{
		return m_Datatype;
	}

}