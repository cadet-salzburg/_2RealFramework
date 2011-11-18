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
#include <map>

namespace _2Real
{

	/**
	*	metadata representation of a param
	*/

	class ParameterMetadata
	{

	public:

		typedef std::map< std::string, std::string >		StringMap;

		/**
		*	yay, typedefs
		*/
		typedef std::pair< std::string, ParameterMetadata >		NamedParameter;

		/**
		*	typedefs, yay
		*/
		typedef std::map< std::string, ParameterMetadata >		ParameterMap;

		/**
		*	ctor will throw if either _name or _type is empty
		*/
		ParameterMetadata(std::string const& _name, std::string const& _type);

		/**
		*	get parameter name
		*
		*	@return:		parameter's name
		*/
		std::string getName() const;

		/**
		*	get parameter type
		*
		*	@return:		parameter's type as string
		*/
		std::string getType() const;

	private:

		/**
		*	name of the param
		*/
		const std::string		m_Name;

		/*
		*	typename as string
		*/
		const std::string		m_Type;

	};

}