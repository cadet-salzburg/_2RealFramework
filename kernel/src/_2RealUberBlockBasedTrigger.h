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

#include "_2RealUpdateTrigger.h"

#include <map>

namespace _2Real
{

	enum BlockMessage
	{
		BLOCK_OK						=	0x00,
		BLOCK_READY						=	0x01,
		BLOCK_FINISHED					=	0x02,
		BLOCK_NOT_OK					=	0x04,
	};

	class AbstractUberBlockBasedTrigger : private UpdateCondition
	{

	public:

		using UpdateCondition::isFullfilled;
		using UpdateCondition::set;
		using UpdateCondition::reset;

		AbstractUberBlockBasedTrigger( std::string const& blockName ) : m_BlockName( blockName ) {}
		virtual ~AbstractUberBlockBasedTrigger() {}
		std::string const& getUberBlockName() { return m_BlockName; }

		virtual bool tryTriggerUpdate( const BlockMessage state ) = 0;

	protected:

		std::string		const m_BlockName;

	};

	class AbstractStateManager;
	typedef std::map< std::string, std::pair< AbstractUberBlockBasedTrigger *, AbstractStateManager * > >	UberBlockBasedTriggerMap;

	template< BlockMessage TriggerState >
	class UberBlockBasedTrigger : public AbstractUberBlockBasedTrigger
	{

	public:

		UberBlockBasedTrigger( std::string const& blockName ) : AbstractUberBlockBasedTrigger( blockName ) {}
		bool tryTriggerUpdate( const BlockMessage state )
		{
			if ( state == TriggerState && !isFullfilled() )
			{
				set();
				return true;
			}
			return false;
		}

	};

}