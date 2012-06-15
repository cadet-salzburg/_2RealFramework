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

	class SystemStates : public AbstractStateManager
	{

	public:

		SystemStates( AbstractBlock &owner );

		void				setUp();
		void				start();
		Poco::Event &		stop();
		void				prepareForShutDown();
		bool				shutDown( const long timeout );
		void				setUpdatePolicy( UpdatePolicyImpl const& policy );

		void tryTriggerTime( long &time );
		void tryTriggerInlet( const void *inlet, std::pair< long, long > &times );
		void tryTriggerSubBlock( AbstractStateManager &sub, const BlockMessage msg );
		void tryTriggerUberBlock( AbstractStateManager &uber, const BlockMessage msg );

		void subBlockAdded( AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg );
		void subBlockRemoved( AbstractBlock &subBlock );
		void uberBlockAdded( AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg );
		void uberBlockRemoved( AbstractBlock &uberBlock );
		void inletAdded( Inlet &inlet, AbstractInletBasedTrigger &trigger );
		void inletRemoved( Inlet &inlet );

	private:

		friend class SystemImpl;

		void setAllowedUpdates( AbstractBlock &block, const unsigned long updates );

		mutable Poco::FastMutex						m_UpdateAccess;
		std::map< std::string, unsigned long >		m_BlockUpdates;

	};

}