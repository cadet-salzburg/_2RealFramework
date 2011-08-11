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

#include "_2RealIService.h"

#include "Poco/Runnable.h"

namespace _2Real
{
	class IContainer : public IService, public Poco::Runnable
	{

	public:

		virtual void start(bool const& _loop) = 0;
		virtual void stop() = 0;
		virtual void run() = 0;
		
		virtual void update() = 0;
		virtual const bool setup(ConfigurationData *const _config) = 0;
		virtual void shutdown() = 0;
		
		virtual void addListener(ServicePtr &_listener) = 0;
		virtual void removeListener(ServicePtr &_listener) = 0;
		virtual void serviceListener(DataPtr &_input) = 0;
		
		virtual void outputData(bool const& _blocking) = 0;
		
		virtual ServiceName const& name() const = 0;

	};
}