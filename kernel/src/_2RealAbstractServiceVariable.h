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

#include "_2RealTypedefs.h"

#include <string>

#include "Poco/Any.h"

namespace _2Real
{

	class AbstractServiceVariable
	{

	public:

		/**
		*
		*/
		AbstractServiceVariable(std::string const& _name, Variable const& _fwName);

		/**
		*
		*/
		AbstractServiceVariable(std::string const& _name);

		/**
		*
		*/
		std::string const& originalName() const;

		/**
		*
		*/
		Variable const& frameworkName() const;

		/**
		*
		*/
		void setFrameworkName(Variable const& _name);

		/**
		*
		*/
		virtual const bool getFrom(VariableData const& _data) = 0;

		/**
		*
		*/
		virtual const bool insertInto(VariableData &_data) const = 0;

		/**
		*
		*/
		const bool getFrom(VariableData const& _data, Poco::Any &_any);

		/**
		*
		*/
		const bool insertInto(VariableData &_data, Poco::Any &_any) const;

	private:

		/**
		*
		*/
		std::string				m_OriginalName;

		/**
		*
		*/
		Variable				m_FrameworkName;

	};

}