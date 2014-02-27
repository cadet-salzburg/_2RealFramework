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

#include "engine/_2RealConditionFactory.h"
#include "engine/_2RealValuesCondition.h"
#include "engine/_2RealTimerCondition.h"
#include "engine/_2RealUserInputCondition.h"

namespace _2Real
{
	std::shared_ptr< Condition > ConditionFactory::createValuesCondition( std::vector< std::shared_ptr< InSlot > > &inputs, std::vector< std::vector< std::string > > &disjunction )
	{
		std::shared_ptr< ValuesCondition > c;
		c->init( inputs, disjunction );
		return c;
	}

	void ConditionFactory::removeValuesCondition( std::shared_ptr< Condition > condition, std::vector< std::shared_ptr< InSlot > > &inputs )
	{
		std::shared_ptr< ValuesCondition > c = std::static_pointer_cast< ValuesCondition >( condition );
		c->cleanup( inputs );
	}
}
