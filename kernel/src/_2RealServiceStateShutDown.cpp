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
#include "_2RealServiceStateShutDown.h"

namespace _2Real
{

	const bool ServiceStateShutDown::setUp(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from shut down to set up");
	}

	void ServiceStateShutDown::triggersReady(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from shut down to waiting");
	}

	void ServiceStateShutDown::beginUpdate(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from shut down to updating");
	}

	void ServiceStateShutDown::beginExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from sht down to executing");
	}

	const bool ServiceStateShutDown::finishExecution(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from shut down to set up");
	}

	const bool ServiceStateShutDown::prepareForShutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from shut down to pre shut down");
	}

	void ServiceStateShutDown::shutDown(ServiceStates &states) const
	{
		throw InvalidStateChangeException("service can't go from shut down to shut down");
	}

}