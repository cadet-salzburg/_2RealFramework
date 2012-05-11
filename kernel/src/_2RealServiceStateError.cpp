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
#include "_2RealServiceStateError.h"

namespace _2Real
{

	const bool ServiceStateError::setUp(ServiceStates &states) const
	{
		//stay where we are
		return false;
	}

	void ServiceStateError::triggersReady(ServiceStates &states) const
	{
		//nothing
	}

	void ServiceStateError::beginUpdate(ServiceStates &states) const
	{
		//nothing
	}

	void ServiceStateError::beginExecution(ServiceStates &states) const
	{
		//nothing
	}

	const bool ServiceStateError::finishExecution(ServiceStates &states) const
	{
		//if this ever happens, it means that the service was shut down & reached a timeout -> go to pre shut down state
		states.flagStopped();
		return true;
	}

	const bool ServiceStateError::prepareForShutDown(ServiceStates &states) const
	{
		//okay.
		states.flagStopped();
		return true;
	}

	void ServiceStateError::shutDown(ServiceStates &states) const
	{
		//nothing
	}

}