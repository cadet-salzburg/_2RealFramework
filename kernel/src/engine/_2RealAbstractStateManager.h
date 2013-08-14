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

#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class EngineImpl;
	class TimeBasedTrigger;
	class AbstractInletBasedTrigger;
	class AbstractUberBlock;
	class EngineImpl;

	class AbstractStateManager : private NonCopyable< AbstractStateManager >
	{

	public:

		virtual ~AbstractStateManager() {}

		virtual std::string const&			getFullHumanReadableName() const = 0;
		virtual std::string const&			getHumanReadableName() const = 0;
		//virtual std::string const&			getCode() const = 0;

		//virtual void triggerTime( long ) = 0;
		////virtual void triggerInlets() = 0;
		//virtual void tryTriggerInlet(/* AbstractInletBasedTrigger &trigger */) = 0;
		//virtual void addTrigger( AbstractInletBasedTrigger &trigger ) = 0;
		//virtual void removeTrigger( AbstractInletBasedTrigger &trigger ) = 0;

		//virtual void scheduleUpdate() = 0;

	//protected:

	//	typedef std::set< AbstractInletBasedTrigger * >						InletTriggers;
	//	typedef std::set< AbstractInletBasedTrigger * >::iterator			InletTriggerIterator;
	//	typedef std::set< AbstractInletBasedTrigger * >::const_iterator		InletTriggerConstIterator;

	};

}