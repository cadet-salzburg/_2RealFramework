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

#include "datatypes/_2RealCustomData.h"

namespace _2Real
{
	// RENAME timestamped->unique or 'DataItem'
	// represents a piece of data inside a queue
	// timestamp + key are used to differentiate

	class TimestampedData
	{

	public:

		TimestampedData() : value(), timestamp( 0 )/*, key( 0 )*/ {}

		// called at 2 places: receiveData - time = current, add inlet - time = 0
		TimestampedData( std::shared_ptr< const CustomType > v, const long t ) : value( v ), timestamp( t )/*, key( 0 )*/ {}

		// timestamped data means the data is already inside some inlet queue -> const, since inlets can write ( maybe in future? )
		std::shared_ptr< const CustomType >		value;
		unsigned long							timestamp;

	private:

		//unsigned long							key;

	};
}