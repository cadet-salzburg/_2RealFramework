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
#include "_2RealException.h"

namespace _2Real
{

	class BadParameterException : public Exception
	{

	public:

		BadParameterException(std::string const& name, std::string const& action) :
			Exception(std::string("parameter : ").append(name).append(" is argh"))
			{
			}

	};

	/**
	*	base parameter class for service-setup / service-input / service-output / plugin-setup
	*/

	class Parameter : public Entity
	{

	public:

		Parameter(Id *const _id, std::string const& type, std::string const& key);
		virtual ~Parameter();

		/**
		*	=?
		*/
		bool const& isInitialized() const
		{
			return m_IsInitialized;
		}

		/**
		*	returns keyword as string
		*/
		std::string const& keyword() const
		{
			return m_Keyword;
		}

		/**
		*	returns typename as string
		*/
		std::string const& datatype() const
		{
			return m_Datatype;
		}

	protected:

		bool						m_IsInitialized;
		const std::string			m_Datatype;
		const std::string			m_Keyword;

	};

}