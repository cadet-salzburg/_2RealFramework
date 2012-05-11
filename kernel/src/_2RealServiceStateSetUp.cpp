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
#include "_2RealServiceStateSetUp.h"

namespace _2Real
{

	const bool ServiceStateSetUp::setUp(ServiceStates &states) const
	{
		//could happen -> just call set up again (triggers already started)
		states.setUpService();
		return true;
	}

	void ServiceStateSetUp::triggersReady(ServiceStates &states) const
	{
		//this is actually the desired state change
	}

	void ServiceStateSetUp::beginUpdate(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin an update when set up");
	}

	void ServiceStateSetUp::beginExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin execution when set up");
	}

	const bool ServiceStateSetUp::finishExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't finish execution when set up");
	}

	const bool ServiceStateSetUp::prepareForShutDown(ServiceStates &states) const
	{
		//could also happen -> disable all triggers & flag the event
		states.disableAllTriggers();
		states.flagStopped();
		return true;
	}

	void ServiceStateSetUp::shutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't shut down when set up");
	}

}