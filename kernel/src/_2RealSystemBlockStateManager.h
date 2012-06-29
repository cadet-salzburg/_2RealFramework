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
#include "_2RealPoco.h"

namespace _2Real
{

	class SystemBlockStateManager : public AbstractStateManager
	{

	public:

		SystemBlockStateManager( AbstractUberBlock &owner );

		void				setUp();
		void				start();
		Poco::Event &		stop();
		void				prepareForShutDown();
		bool				shutDown( const long timeout );

		void tryTrigger( AbstractUpdateTrigger &trigger );
		void tryTriggerUberBlock( UberBlockBasedTrigger &trigger );

		void addTrigger( AbstractUpdateTrigger &trigger );
		void removeTrigger( AbstractUpdateTrigger &trigger );
		void addUberBlockTrigger( UberBlockBasedTrigger &trigger );
		void removeUberBlockTrigger( UberBlockBasedTrigger &trigger );

	private:

		Poco::FastMutex									m_Access;
		AbstractStateManager::UberBlockTriggerList		m_SubBlockTriggers;

	};

}