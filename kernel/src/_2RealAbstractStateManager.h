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

#include "_2RealUberBlockBasedTrigger.h"
#include "_2RealInletBasedTrigger.h"
#include "_2RealTimeBasedTrigger.h"

#include <string>

#include "Poco/Event.h"

namespace _2Real
{

	class AbstractUberBlock;
	class Inlet;
	class UpdatePolicyImpl;

	class AbstractStateManager
	{

	public:

		AbstractStateManager(AbstractUberBlock &owner);
		virtual ~AbstractStateManager();

		std::string const& getName() const;
		unsigned int getId() const;

		virtual void setUp() = 0;
		virtual void start() = 0;
		virtual Poco::Event & stop() = 0;
		virtual void prepareForShutDown() = 0;
		virtual bool shutDown( const long timeout ) = 0;
		virtual void setUpdatePolicy( UpdatePolicyImpl const& policy ) = 0;

		virtual void tryTriggerTime( long &time ) = 0;
		virtual void tryTriggerInlet( const void *inlet, std::pair< long, long > &times ) = 0;
		virtual void tryTriggerSubBlock( const unsigned int id, const BlockMessage msg ) = 0;
		virtual void tryTriggerSuperBlock( const unsigned int id, const BlockMessage msg ) = 0;

		virtual void addTriggerForSubBlock( const unsigned int id, AbstractUberBlockBasedTrigger &trigger ) = 0;
		virtual void removeTriggerForSubBlock( const unsigned int id ) = 0;
		virtual void addTriggerForSuperBlock( const unsigned int id, AbstractUberBlockBasedTrigger &trigger ) = 0;
		virtual void removeTriggerForSuperBlock( const unsigned int id ) = 0;

	protected:

		AbstractUberBlock					&m_Owner;
		Poco::Event						m_StopEvent;

	};

}