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

#include "app/_2RealCommon.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class AbstractInletIO;
	class CustomType;
	class UpdatePolicy;

	namespace app
	{
		class TypeMetainfo;
		class OutletHandle;
		class LinkHandle;
		class InputMetainfo;

		class InletHandle
		{

		public:

			InletHandle();
			InletHandle( std::shared_ptr< AbstractInletIO > );

			// check this if you're unsure if the underlying inlet still exists
			bool									isValid() const;

			InputMetainfo							getInletMetainfo() const;
			TypeMetainfo							getTypeMetainfo() const;

			// don't use until some other things are sorted out
			//void									setUpdatePolicy( UpdatePolicy const& );
			//void									setBufferSize( const unsigned int );

			// linking
			LinkHandle								link( OutletHandle );
			void									unlinkFrom( OutletHandle );

			// creates a writeable data item
			std::shared_ptr< CustomType >			makeData() const;
			std::shared_ptr< const CustomType >		getCurrentData() const;
			// bypasses the buffer, sets data directly...
			// this makes very little sense when the inlet is linked to sth else
			void									setData( std::shared_ptr< const CustomType > );
			// inserts data into buffer
			void									receiveData( std::shared_ptr< const CustomType > );

			// multi-inlet related stuff -> each of those functions has a chance of failing
			bool									canExpand() const;
			unsigned int							getSubInletCount() const;
			InletHandle								operator[]( const unsigned int );
			InletHandle								add();
			void									remove( InletHandle handle );

		private:

			friend class OutletHandle;
			std::weak_ptr< AbstractInletIO >		mImpl;

		};
	}
}
