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

#include <sstream>

namespace _2Real
{
	class AbstractInlet;

	namespace bundle
	{
		class InletHandle
		{

		public:

			InletHandle();
			InletHandle( AbstractInlet &inlet );
			InletHandle( InletHandle const& src );
			InletHandle& operator=( InletHandle const& src );
			~InletHandle();

			bool isValid() const;
			void invalidate();

			// true if inlet was declared as mutli inlet according to metadata
			bool isMultiInlet() const;
			// if not multi: returns 1
			unsigned int getSize() const;

			bool hasUpdated() const;
			bool hasChanged() const;

			InletHandle operator[]( const unsigned int index );

			std::shared_ptr< const CustomType > getReadableRef() const;
			void getWriteableCopy( CustomType &type ) const;

		private:

			AbstractInlet		*m_Inlet;

		};
	}
}