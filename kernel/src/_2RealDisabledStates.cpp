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

#include "_2RealDisabledStates.h"

namespace _2Real
{

	DisabledStates::DisabledStates(AbstractBlock &owner) :
		AbstractStateManager(owner)
	{
	}

	DisabledStates::~DisabledStates()
	{
		clear();
	}

	void DisabledStates::clear()
	{
	}

	void DisabledStates::setUp()
	{
	}

	void DisabledStates::prepareForShutDown()
	{
	}

	const bool DisabledStates::shutDown()
	{
		return true;
	}

	void DisabledStates::tryTriggerInlet(const void *inlet, std::pair< long, long > &times)
	{
	}

	void DisabledStates::tryTriggerTime(long &time)
	{
	}

	void DisabledStates::tryTriggerSubBlock(AbstractStateManager &sub, const BlockMessage desiredMsg)
	{
	}

	void DisabledStates::tryTriggerUberBlock(AbstractStateManager &uber, const BlockMessage desiredMsg)
	{
	}

	void DisabledStates::subBlockAdded(AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	{
	}

	void DisabledStates::subBlockRemoved(AbstractBlock &subBlock)
	{
	}

	void DisabledStates::uberBlockAdded(AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	{
	}

	void DisabledStates::uberBlockRemoved(AbstractBlock &uberBlock)
	{
	}

	void DisabledStates::inletAdded(Inlet &slot, AbstractInletBasedTrigger &trigger)
	{
	}

	void DisabledStates::inletRemoved(Inlet &slot)
	{
	}

}