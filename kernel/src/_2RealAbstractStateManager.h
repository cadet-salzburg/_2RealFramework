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

#include "app/_2RealUpdatePolicyHandle.h"
#include "_2RealPoco.h"

#include <string>
#include <list>

namespace _2Real
{

	class AbstractUpdateTrigger;
	class UberBlockBasedTrigger;
	class AbstractUberBlock;
	class UpdatePolicy;

	class AbstractStateManager
	{

	public:

		typedef std::list< AbstractUpdateTrigger * >			TriggerList;
		typedef std::list< UberBlockBasedTrigger * >	UberBlockTriggerList;

		AbstractStateManager( AbstractUberBlock &owner );
		virtual ~AbstractStateManager();

		std::string const& getName() const;
		unsigned int getId() const;

		virtual void setUp() = 0;
		virtual void start() = 0;
		virtual Poco::Event & stop() = 0;
		virtual void prepareForShutDown() = 0;
		virtual bool shutDown( const long timeout ) = 0;

		virtual void tryTrigger( AbstractUpdateTrigger &trigger ) = 0;
		virtual void tryTriggerUberBlock( UberBlockBasedTrigger &trigger ) = 0;

		virtual void addTrigger( AbstractUpdateTrigger &trigger ) = 0;
		virtual void removeTrigger( AbstractUpdateTrigger &trigger ) = 0;
		virtual void addUberBlockTrigger( UberBlockBasedTrigger &trigger ) = 0;
		virtual void removeUberBlockTrigger( UberBlockBasedTrigger &trigger ) = 0;

	protected:

		AbstractUberBlock		&m_Owner;
		Poco::Event				m_StopEvent;

	};

}