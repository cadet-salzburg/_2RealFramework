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

/**
*	annoying & ultimately harmless warning about
*	template expressions that are longer than 255 bytes
*/
#pragma warning(disable:4503)

#include <vector>

namespace _2Real
{

	class Data;
	class Identifier;
	class RunnableException;


	/**
	*	callback for exceptions
	*
	*	there are 3 entities which allow for a exception callback registration:
	*	services, sequences & synchronizations - see @registerToException
	*/
	typedef void (*ExceptionCallback)(RunnableException &exception);

	/**
	*	callback for new data
	*
	*	there are 3 entities which allow for a exception callback registration:
	*	services, sequences & synchronizations - see @registerToNewData
	*/
	typedef void (*DataCallback)(Data &_data);

	/**
	*	a vector of identifiers
	*
	*	is the return value of some functions:
	*		- plugins return the ids of all their services on installation
	*		- services can be queried for the ids of setup params
	*		- services, sequences & synchronizations can be queried for the ids of IO slots
	*		- sequences & synchronizations can be queries for the ids of children
	*/
	typedef std::vector< Identifier > Identifiers;
}