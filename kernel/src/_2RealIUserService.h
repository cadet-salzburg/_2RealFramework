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

namespace _2Real
{

	class ServiceContext;

	class IUserService
	{

	public:

		/**
		*	register setup params / input variables / putput variables
		*/
		virtual void setup(ServiceContext *const _context) = 0;

		/**
		*	initialization
		*/
		virtual const bool init() = 0;

		/**
		*	cleanup
		*/
		virtual void shutdown() = 0;
		
		/**
		*	update
		*/
		virtual void update() = 0;
	
	};

}