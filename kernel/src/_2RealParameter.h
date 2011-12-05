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

#include "_2RealEntity.h"

namespace _2Real
{

	class Parameter : public Entity
	{

	public:

		Parameter(Identifier const& id, std::string const& type, std::string const& key);
		virtual ~Parameter();

		bool					isInitialized() const;
		std::string const&		keyword() const;
		std::string const&		datatype() const;

	protected:

		bool					m_IsInitialized;
		const std::string		m_Datatype;
		const std::string		m_Keyword;

	};

	inline bool Parameter::isInitialized() const
	{
		return m_IsInitialized;
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