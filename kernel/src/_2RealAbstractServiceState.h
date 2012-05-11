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

#include <string>
#include <sstream>

namespace _2Real
{

	class ServiceStates;

	class AbstractServiceState
	{

	public:

		virtual ~AbstractServiceState() {}

		/**
		*	may called by the user at any time
		*	if the return value is false, this means that no state change will take place
		*	(b/c the service is in a execution cycle)
		*	otherwise, the follow up state is 'set up'
		*	in shut down / pre shut down, throws
		*/
		virtual const bool setUp(ServiceStates &states) const = 0;

		/**
		*	called by a service's triggers, as soon as they are all fulfilled
		*	only valid for 'set up', the follow up state is 'updating'
		*	once a service has reached updating state, there is no way to set up or shut it down until
		*	it has passed through 'updating' and 'executing' state
		*/
		virtual void triggersReady(ServiceStates &states) const = 0;

		/**
		*	called by a service's triggers, as soon as they are all fulfilled
		*	only valid for 'set up', the follow up state is 'updating'
		*	once a service has reached updating state, there is no way to set up or shut it down until
		*	it has passed through 'updating' and 'executing' state
		*/
		virtual void beginUpdate(ServiceStates &states) const = 0;

		/**
		*	called by the thread pool as soon as there is a free thread
		*	only valid for 'updating', the follow up state is 'executing'
		*/
		virtual void beginExecution(ServiceStates &states) const = 0;

		/**
		*	called by the thread pool once execution is finished
		*	when this returns true, it means that the follow up state should be 'set up'
		*	else the follow up state should be 'pre shut down'
		*/
		virtual const bool finishExecution(ServiceStates &states) const = 0;

		/**
		*	called by the user at any time
		*/
		virtual const bool prepareForShutDown(ServiceStates &states) const = 0;

		/**
		*	shut down is always called after pre shut down
		*	it will wait for up to a second for the service to complete
		*/
		virtual void shutDown(ServiceStates &states) const = 0;

	};

}