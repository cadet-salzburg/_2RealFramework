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

#include <string>

namespace _2Real
{
	class BasicInletIO;
	class OutletIO;
	class AbstractUberBlock;
	class TypeConverter;

	class IOLink
	{

	public:

		IOLink();	/* creates invalid link! */
		~IOLink();

		void activate();
		void deactivate();

		BasicInletIO const& getInletIO() const;
		OutletIO const& getOutletIO() const;

		BasicInletIO& getInletIO();
		OutletIO& getOutletIO();

		bool operator<( IOLink const& other );
		bool isBlockInvolved( AbstractUberBlock const& b ) const;
		bool isInletInvolved( BasicInletIO const& inlet ) const;

		bool isValid() const;

		static IOLink * link( BasicInletIO &inlet, OutletIO &outlet );
		//static IOLink * linkWithAutoConversion( BasicInletIO &inlet, OutletIO &outlet );

		//static const std::string findConversion( BasicInletIO &inlet, OutletIO &outlet );
		//static bool canAutoConvert( BasicInletIO &inlet, OutletIO &outlet );

		void receiveData( TimestampedData const& );

	private:

		IOLink( BasicInletIO &inlet, OutletIO &outlet, TypeConverter const* cv );

		BasicInletIO		*m_InletIO;
		OutletIO			*m_OutletIO;
		TypeConverter		const* mConverter;

	};
}