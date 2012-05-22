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

#include "_2RealException.h"
#include "_2RealServiceBlock.h"
#include "_2RealServiceStateCreated.h"

#include <iostream>

namespace _2Real
{

	const bool ServiceStateCreated::setUp(ServiceStates &states) const
	{
		//the expected state change -> set up & start the normal triggers
		states.setUpService();
		return true;
	}

	void ServiceStateCreated::triggersReady(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't wait for uber triggers in newly created state");
	}

	void ServiceStateCreated::beginUpdate(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin an update in newly created state");
	}

	void ServiceStateCreated::beginExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin execution in newly created state");
	}

	const bool ServiceStateCreated::finishExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't finish an update in newly created state");
	}

	const bool ServiceStateCreated::prepareForShutDown(ServiceStates &states) const
	{
		//this can happen, too -> no problem here, service is def. not in execution anyway
		states.disableAllTriggers();
		states.flagStopped();
		return true;
	}

	void ServiceStateCreated::shutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't be shut down in newly created state");
	}

}