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

#include "_2RealTriggerTypes.h"

#include <string>

#include "Poco/Event.h"

namespace _2Real
{

	class AbstractBlock;
	class Inlet;
	class UpdatePolicyImpl;

	class AbstractStateManager
	{

	public:

		AbstractStateManager(AbstractBlock &owner);
		virtual ~AbstractStateManager();

		std::string const& getName() const;
		unsigned int getId() const;

		virtual void setUp() = 0;
		virtual void start() = 0;
		virtual Poco::Event & stop() = 0;
		virtual void prepareForShutDown() = 0;
		virtual bool shutDown( const long timeout ) = 0;
		virtual void setUpdatePolicy( UpdatePolicyImpl const& policy ) = 0;

		//called by the timer thread
		virtual void tryTriggerTime(long &time) = 0;
		//called by the individual inlets
		virtual void tryTriggerInlet(const void *inlet, std::pair< long, long > &times) = 0;
		//called when a certain sub block is ready or completed
		virtual void tryTriggerSubBlock(AbstractStateManager &sub, const BlockMessage msg) = 0;
		//called when an uber block is ready
		virtual void tryTriggerUberBlock(AbstractStateManager &uber, const BlockMessage msg) = 0;

		//relevant for group blocks
		//virtual void subBlockAdded(AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger) = 0;
		//virtual void subBlockRemoved(AbstractBlock &subBlock) = 0;
		virtual void subBlockAdded(AbstractBlock &sub, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg) = 0;
		virtual void subBlockRemoved(AbstractBlock &sub) = 0;
		//relevant for any block
		//virtual void uberBlockAdded(AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger) = 0;
		//virtual void uberBlockRemoved(AbstractBlock &uberBlock) = 0;
		virtual void uberBlockAdded(AbstractBlock &uber, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg) = 0;
		virtual void uberBlockRemoved(AbstractBlock &uber) = 0;
		//relevant for blocks which can add inlets
		//virtual void inletAdded(Inlet &slot, AbstractInletBasedTrigger &trigger) = 0;
		//virtual void inletRemoved(Inlet &slot) = 0;

	protected:

		AbstractBlock					&m_Owner;
		Poco::Event						m_StopEvent;

	};

}