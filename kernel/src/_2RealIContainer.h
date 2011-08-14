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

#include "_2RealIService.h"
#include "_2RealIDataQueue.h"

#include "Poco/Runnable.h"

namespace _2Real
{

	/**
	*
	*/

	class ConfigurationData;

	class IContainer : public IService, public IDataQueue, public Poco::Runnable
	{

	public:

		/**
		*
		*/
		virtual void start(bool const& _runOnce) = 0;

		/**
		*
		*/
		virtual void stop() = 0;

		/**
		*	checks if container can be reconfigured
		*/
		virtual bool const& canBeReconfigured() const = 0;

		/**
		*	reconfigures container
		*/
		virtual void configure(ConfigurationData *const _data) throw(...) = 0;

	};

}