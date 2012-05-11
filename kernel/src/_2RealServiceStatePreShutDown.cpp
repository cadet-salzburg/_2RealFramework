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
#include "_2RealServiceStatePreShutDown.h"

namespace _2Real
{

	const bool ServiceStatePreShutDown::setUp(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't be set up when preparing s shut down");
	}

	void ServiceStatePreShutDown::triggersReady(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't start waiting when preparing a shut down");
	}

	void ServiceStatePreShutDown::beginUpdate(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin an update when preparing a shut down");
	}

	void ServiceStatePreShutDown::beginExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't begin execution when preparing a shut down");
	}

	const bool ServiceStatePreShutDown::finishExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't finish execution when preparing a shut down");
	}

	const bool ServiceStatePreShutDown::prepareForShutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from pre shut down to pre shut down");
	}

	void ServiceStatePreShutDown::shutDown(ServiceStates &states) const
	{
		states.shutDownService();
	}

}