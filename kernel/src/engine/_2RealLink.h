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

#include "engine/_2RealTimestampedData.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class BasicInletIO;
	class OutletIO;
	class AbstractUberBlock;
	class TypeConverter;

	class IOLink
	{

	public:

		// dummy link!
		IOLink( std::shared_ptr< BasicInletIO > i, std::shared_ptr< OutletIO > o ) : mInletIO( i ), mOutletIO( o ) {}

		// creates real link
		static std::shared_ptr< IOLink > link( std::shared_ptr< BasicInletIO >, std::shared_ptr< OutletIO > );

		void activate();
		void deactivate();

		// y?
		bool operator<( IOLink const& other );

		void receiveData( TimestampedData const& );

	private:

		IOLink( std::shared_ptr< BasicInletIO >, std::shared_ptr< OutletIO >, std::shared_ptr< const TypeConverter >, std::shared_ptr< const TypeConverter > );

		std::shared_ptr< BasicInletIO >			mInletIO;
		std::shared_ptr< OutletIO >				mOutletIO;
		std::shared_ptr< const TypeConverter >	mForwardConverter;
		std::shared_ptr< const TypeConverter >	mReverseConverter;

	};
}