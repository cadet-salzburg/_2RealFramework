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

#include "Poco/SharedPtr.h"
#include <string>

namespace _2Real
{

	/**
	*
	*/
	template < typename Key >
	class Data;

	/**
	*
	*/
	class IdentifierImpl;

	/**
	*
	*/
	typedef Data< IdentifierImpl >							VariableData;

	typedef Data< std::string >								ParameterData;

	/**
	*
	*/
	typedef Poco::SharedPtr< VariableData >				SharedDataPtr;

	/**
	*
	*/
	typedef std::pair< IdentifierImpl, SharedDataPtr >		NamedData;

}