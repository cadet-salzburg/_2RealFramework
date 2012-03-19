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

#include "_2RealRunnableTriggers.h"

#include <string>

namespace _2Real
{

	class AbstractInletDefault
	{

	public:

		virtual AbstractInletBasedTrigger * createTrigger(std::string const& name) = 0;

	};

	template< typename Comparison >
	class InletDefault : public AbstractInletDefault
	{

	public:

		AbstractInletBasedTrigger * createTrigger(std::string const& name)
		{
			return new InletBasedTrigger< Comparison >(name);
		}

	};

	class AbstractChildDefault
	{

	public:

		virtual AbstractChildBasedTrigger * createTrigger(std::string const& name) = 0;

	};

	template< unsigned int State >
	class ChildDefault : public AbstractChildDefault
	{

	public:

		AbstractChildBasedTrigger * createTrigger(std::string const& name)
		{
			return new ChildBasedTrigger< State >(name);
		}

	};

	class UpdateTriggersImpl
	{

	public:

		UpdateTriggersImpl();
		~UpdateTriggersImpl();

		void clear();

		void setInletDefault(AbstractInletDefault *inletDefault);
		void setChildDefault(AbstractChildDefault *childDefault);

		void addInletBasedTrigger(std::string const& inletName, AbstractInletDefault *trigger);
		void addTimeBasedTrigger(const long time);
		void addChildBasedTrigger(std::string const& childName, AbstractChildDefault *trigger);

		AbstractTimeBasedTrigger * getTimeBasedTrigger();
		AbstractInletBasedTrigger * getInletBasedTrigger(std::string const& name);
		AbstractChildBasedTrigger * getChildBasedTrigger(std::string const& name);

	private:

		//AbstractTimeBasedTrigger	*m_Time;
		long						m_Time;
		bool						m_HasTime;

		AbstractInletDefault		*m_InletDefault;
		AbstractChildDefault		*m_ChildDefault;

		std::map< std::string, AbstractInletDefault * > m_Inlets;
		std::map< std::string, AbstractChildDefault * > m_Children;

	};

}