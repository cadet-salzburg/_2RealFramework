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

#include "engine/_2RealValuesCondition.h"
#include "engine/_2RealInSlot.h"

namespace _2Real
{
	void ValuesCondition::init( std::vector< std::shared_ptr< InSlot > > inputs, std::vector< std::vector< std::string > > &disjunction )
	{
		mFlags.resize( inputs.size(), false );

		mDisjunction.reserve( disjunction.size() );
		for ( auto conjunction : disjunction )
		{
			std::vector< unsigned int > tmp; tmp.reserve( conjunction.size() );
			for ( auto name : conjunction )
				for ( unsigned int id=0; id<inputs.size(); ++id )
					if ( inputs[ id ]->getName() == name )
						tmp.push_back( id );
			mDisjunction.push_back( tmp );
		}

		for ( auto in : inputs )
			in->registerToValueUpdated( this, &ValuesCondition::newValueReceived );
	}

	void ValuesCondition::cleanup( std::vector< std::shared_ptr< InSlot > > inputs )
	{
		for ( auto in : inputs )
			in->unregisterFromValueUpdated( this, &ValuesCondition::newValueReceived );
	}

	void ValuesCondition::newValueReceived( const unsigned int id )
	{
		if ( mFlags[ id ] ) return;
		mFlags[ id ] = true;

		bool isDisjunctionTrue = false;
		for ( auto conjunction : mDisjunction )
		{
			bool isConjunctionTrue = true;
			for ( auto var : conjunction )
			{
				isConjunctionTrue &= mFlags[ var ];
				if ( !isConjunctionTrue ) break;
			}

			isDisjunctionTrue |= isConjunctionTrue;
			if ( isDisjunctionTrue ) break;
		}

		if ( isDisjunctionTrue )
			Condition::isFulfilled();
	}
}
