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

#include <list>

namespace _2Real
{

	class Data;
	class Identifier;
	class Exception;
	class IService;

	/**
	*	callback for exceptions
	*
	*	there are 3 entities which allow for a exception callback registration:
	*	services, sequences & synchronizations - see @registerToException
	*
	*	registering for exception callbacks really only makes sense if the entity is in
	*	nirvana
	*/
	typedef void (*ExceptionCallback)(Identifier const& _sender, Exception const& _exception);

	/**
	*	callback for new DataImpl
	*
	*	there are 3 entities which allow for a exception callback registration:
	*	services, sequences & synchronizations - see @registerToNewData
	*
	*	as with the exceptions, registering for new DataImpl makes the most sense if the entity
	*	in question is in nirvana
	*/
	typedef void (*NewDataCallback)(std::pair< Identifier, Data > &_data);

	/**
	*	a list of identifiers
	*
	*	is the return value of some functions:
	*		- plugins return the ids of all their services on installation
	*		- services return their setup params on creation
	*		- services, sequences & synchronizations can be queried for their IO slots
	*/
	typedef std::list< Identifier > Identifiers;

	/**
	*	function to create user defined service - exported by plugins
	*/
	typedef IService *const (*const ServiceCreator)(void);

}