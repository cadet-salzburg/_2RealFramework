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

#include "_2RealAbstractStateManager.h"

#include "Poco/Mutex.h"

namespace _2Real
{

	class SystemBlockStateManager : public AbstractStateManager
	{

	public:

		SystemBlockStateManager( AbstractUberBlock &owner );
		~SystemBlockStateManager();

		void				setUp();
		void				start();
		Poco::Event &		stop();
		void				prepareForShutDown();
		bool				shutDown( const long timeout );
		void				setUpdatePolicy( UpdatePolicyImpl const& policy );

		void tryTriggerTime( long &time );
		void tryTriggerInlet( const void *inlet, std::pair< long, long > &times );
		void tryTriggerSubBlock( const unsigned int id, const BlockMessage msg );
		void tryTriggerSuperBlock( const unsigned int id, const BlockMessage msg );

		void addTriggerForSubBlock( const unsigned int id, AbstractUberBlockBasedTrigger &trigger );
		void removeTriggerForSubBlock( const unsigned int id );
		void addTriggerForSuperBlock( const unsigned int id, AbstractUberBlockBasedTrigger &trigger );
		void removeTriggerForSuperBlock( const unsigned int id );

	private:

		mutable Poco::FastMutex				m_TriggerAccess;
		UberBlockBasedTriggerMap			m_SubBlockTriggers;

	};

}