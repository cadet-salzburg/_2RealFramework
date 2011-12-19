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

namespace _2Real
{

	/**
	*	interface for services
	*/

	class ServiceContext;

	class IService
	{

	public:

		/**
		*	the setup function, which will be executed at least once before the service can run, but might be executed multiple times
		*
		*	the service can use the service context to initialized its handles & query its setup parameters
		*	during setup, a ServiceSetupException should be thrown if something occurs from which you can not recover in any way
		*	while an InvalidParameterException can be thrown otherwise
		*	
		*	@param context		the service context, used for registering handles & querying setup parameters
		*	@throw				ServiceException: something is horribly wrong and the service will never ever run -> signals
		*						to the framework that this service is kaputt
		*	@throw				SetupFailedException: something went wrong but setup could succeed later (typical: some setup param was not set)
		*/
		virtual void setup(ServiceContext &context) = 0;

		/**
		*	update function
		*/
		virtual void update() = 0;

		/**
		*	shutdown function
		*/
		virtual void shutdown() = 0;

		/**
		*	destructor
		*/
		virtual ~IService() = 0;

	};
	
	inline IService::~IService() {}

}