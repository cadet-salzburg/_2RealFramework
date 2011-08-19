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

#include <list>

namespace _2Real
{

	class Identifier;
	class Exception;
	class OutputData;

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
	*	callback for new data
	*
	*	there are 3 entities which allow for a exception callback registration:
	*	services, sequences & synchronizations - see @registerToNewData
	*
	*	as with the exceptions, registering for new data makes the most sense if the entity
	*	in question is in nirvana
	*/
	typedef void (*NewDataCallback)(Identifier const& _sender, OutputData const& _data);

	/**
	*	a list of identifiers
	*
	*	is the return value of some functions:
	*		- plugins return the ids of all their services on installation
	*		- services return their setup params on creation
	*		- services, sequences & synchronizations can be queried for their IO slots
	*/
	typedef std::list< Identifier > Identifiers;

}