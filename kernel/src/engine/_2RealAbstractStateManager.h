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

#include "helpers/_2RealPoco.h"
#include "helpers/_2RealNonCopyable.h"

#include <string>
#include <set>

namespace _2Real
{

	class AbstractUpdateTrigger;
	class AbstractInletBasedTrigger;
	class AbstractTimeBasedTrigger;
	class UberBlockBasedTrigger;
	class AbstractUberBlock;

	class AbstractStateManager : private NonCopyable< AbstractStateManager >
	{

	public:

		//typedef std::list< UberBlockBasedTrigger * >		UberBlockTriggerList;

		AbstractStateManager( AbstractUberBlock &owner );
		virtual ~AbstractStateManager();

		std::string const&	getName() const;
		unsigned int		getId() const;

		virtual void tryTriggerInlet( AbstractInletBasedTrigger &trigger ) = 0;
		virtual void tryTriggerTime( AbstractTimeBasedTrigger &trigger ) = 0;
		//virtual void tryTriggerUberBlock( UberBlockBasedTrigger &trigger ) = 0;

		virtual void addTrigger( AbstractTimeBasedTrigger &trigger ) = 0;
		virtual void removeTrigger( AbstractTimeBasedTrigger &trigger ) = 0;
		virtual void addTrigger( AbstractInletBasedTrigger &trigger ) = 0;
		virtual void removeTrigger( AbstractInletBasedTrigger &trigger ) = 0;
		//virtual void addUberBlockTrigger( UberBlockBasedTrigger &trigger ) = 0;
		//virtual void removeUberBlockTrigger( UberBlockBasedTrigger &trigger ) = 0;

	protected:

		typedef std::set< AbstractInletBasedTrigger * >						InletTriggers;
		typedef std::set< AbstractInletBasedTrigger * >::iterator			InletTriggerIterator;
		typedef std::set< AbstractInletBasedTrigger * >::const_iterator		InletTriggerConstIterator;

		AbstractUberBlock		&m_Owner;
		Poco::Event				m_StopEvent;

	};

}