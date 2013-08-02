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

#include "helpers/_2RealException.h"
#include "datatypes/_2RealCustomData.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class AbstractInlet;

	namespace bundle
	{
		class InletHandle
		{

		public:

			InletHandle();
			explicit InletHandle( std::shared_ptr< AbstractInlet > );

			bool isValid() const;

			// true if inlet was declared as mutli inlet according to metadata
			bool canExpand() const;
			// if not multi: returns 0
			unsigned int getSubInletCount() const;

			bool hasUpdated() const;
			bool hasChanged() const;

			InletHandle operator[]( const unsigned int index );

			std::shared_ptr< const CustomType > getReadableRef() const;
			std::shared_ptr< CustomType > getWriteableCopy() const;

		private:

			std::weak_ptr< AbstractInlet >		mImpl;

		};
	}
}