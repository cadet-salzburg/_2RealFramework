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
#include "_2RealServiceStateUpdating.h"

namespace _2Real
{

	const bool ServiceStateUpdating::setUp(ServiceStates &states) const
	{
		//could happen -> no state change (must finish execution cycle first)
		states.flagForSetUp();
		return false;
	}

	void ServiceStateUpdating::triggersReady(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin waiting for uber triggers when updating");
	}

	void ServiceStateUpdating::beginUpdate(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin an update when updating");
	}

	void ServiceStateUpdating::beginExecution(ServiceStates &states) const
	{
		//the desired state change
	}

	const bool ServiceStateUpdating::finishExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't finsih execution when updating");
	}

	const bool ServiceStateUpdating::prepareForShutDown(ServiceStates &states) const
	{
		//could happen -> no state change (must finish execution cycle first)
		states.flagForShutDown();
		return false;
	}

	void ServiceStateUpdating::shutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't shut down when updating");
	}

	///////////////////////////////////

	const bool ServiceStateWaiting::setUp(ServiceStates &states) const
	{
		//could happen -> no state change (must finish execution cycle first)
		states.flagForSetUp();
		return false;
	}

	void ServiceStateWaiting::triggersReady(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin waiting for uber triggers when waiting");
	}

	void ServiceStateWaiting::beginUpdate(ServiceStates &states) const
	{
		//the desired state change
	}

	void ServiceStateWaiting::beginExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin execution when waiting");
	}

	const bool ServiceStateWaiting::finishExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't finsih execution when waiting");
	}

	const bool ServiceStateWaiting::prepareForShutDown(ServiceStates &states) const
	{
		states.flagForShutDown();
		return false;
	}

	void ServiceStateWaiting::shutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't shut down when waiting");
	}

}