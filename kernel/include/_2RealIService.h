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

#include "_2RealFramework.h"
#include "_2RealConfigMetadata.h"

#include "Poco/Runnable.h"

/*
	service interface
*/

namespace _2Real
{

	class IService
	{

	public:

		virtual bool setup(ConfigMetadataPtr const& _config) = 0;		//initialization function
		virtual void shutdown() = 0;									//cleanup function
		virtual void update() = 0;										//update function (former run())

		virtual void addListener(ServicePtr _listener) = 0;				//add other service to listerners
		virtual void removeListener(ServicePtr _listener) = 0;			//remove other service from listeners
		virtual void serviceListener(DataPtr &_input) = 0;				//react to new data sent by other service
		virtual void outputData(bool _blocking) = 0;					//send new data to listeners, either blocking or nonblocking
	
	};
}