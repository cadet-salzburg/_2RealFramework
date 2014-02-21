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

#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class DefaultPolicy;

	namespace bundle
	{
		class InSlotPolicy
		{
		public:
			virtual ~InSlotPolicy() {}
			virtual operator std::shared_ptr< DefaultPolicy > () const = 0;
		};

		class VALUES_NEW : public InSlotPolicy
		{

		public:

			enum Code { ALL, ANY };

			VALUES_NEW( const Code c );
			VALUES_NEW( std::vector< std::vector< std::string > > const& names );

			explicit operator std::shared_ptr< DefaultPolicy > () const;

		private:

			std::shared_ptr< DefaultPolicy > mPolicy;

		};
	}
}