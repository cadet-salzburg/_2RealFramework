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
#include "engine/_2RealInletPolicy.h"

namespace _2Real
{
	class AbstractInletIO;

	namespace app
	{
		class OutletHandle;
		class BlockHandle;

		class InletHandle
		{

		public:

			InletHandle();
			InletHandle( std::shared_ptr< AbstractInletIO > );

			bool isValid() const;

			std::string const&				getName() const;
			app::TypeMetainfo				getType() const;

			std::shared_ptr< CustomType >	makeData() const;

			// MISSING: function for querying update policy, current state, buffer size

			void				setUpdatePolicy( Policy const& );
			void				setBufferSize( const unsigned int );

			bool				link( OutletHandle );
			void				unlinkFrom( OutletHandle );

			std::shared_ptr< const CustomType >		getCurrentData() const;
			void									receiveData( std::shared_ptr< const CustomType > );

			// multi-inlet related stuff -> each of those functions has a chance of failing

			bool isMultiInlet() const;
			unsigned int getSize() const;
			InletHandle operator[]( const unsigned int index );
			InletHandle add();
			void remove( InletHandle handle );

		private:

			friend class OutletHandle;

			std::weak_ptr< AbstractInletIO >		mImpl;

		};
	}
}
