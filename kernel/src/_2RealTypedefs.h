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

namespace _2Real
{

	/**
	*	dun dun dun - its a service
	*/
	class IService;

	/**
	*	function to create user defined service - exported by plugins
	*/
	typedef IService *const (*ServiceCreator)(void);

	/**
	*	different types of containers
	*/
	enum eContainerType
	{
		PRODUCTION,
		SEQUENCE,
		SYNCHRONIZATION,
	};

}