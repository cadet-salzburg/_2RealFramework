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
#include "_2RealServiceStateExecuting.h"

namespace _2Real
{

	const bool ServiceStateExecuting::setUp(ServiceStates &states) const
	{
		states.flagForSetUp();
		return false;
	}

	void ServiceStateExecuting::triggersReady(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin waiting for uber triggers when executing");
	}

	void ServiceStateExecuting::beginUpdate(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin an update when executing");
	}

	void ServiceStateExecuting::beginExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin execution when executing");
	}

	const bool ServiceStateExecuting::finishExecution(ServiceStates &states) const
	{
		//the expected state change
		return states.conditionalSetUp(); //if this returns false, the next state will be pre shut down; else it will be set up
	}

	const bool ServiceStateExecuting::prepareForShutDown(ServiceStates &states) const
	{
		states.flagForShutDown();
		return false;
	}

	void ServiceStateExecuting::shutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service shut down when executing");
	}

}