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

#include "_2RealAbstractServiceBlockState.h"

namespace _2Real
{

	class ServiceStateExecuting : public AbstractServiceState
	{

	public:

		const bool setUp(ServiceStates &states) const;
		void triggersReady(ServiceStates &states) const;
		void beginUpdate(ServiceStates &states) const;
		void beginExecution(ServiceStates &states) const;
		const bool finishExecution(ServiceStates &states) const;
		const bool prepareForShutDown(ServiceStates &states) const;
		void shutDown(ServiceStates &states) const;

	};

}