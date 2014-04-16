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

#include "app/_2RealOutletHandle.h"
#include "engine/_2RealOutletImpl.h"

#include "common/_2RealWeakPtrCheck.h"

namespace _2Real
{
	namespace app
	{
		OutletHandle::OutletHandle( std::shared_ptr< OutletImpl > outlet ) :
			mImpl( outlet )
		{
		}

		bool OutletHandle::isValid() const
		{
			std::shared_ptr< OutletImpl > outlet = mImpl.lock();
			return ( nullptr != outlet.get() );
		}

		OutletHandle::operator std::shared_ptr< DataSource_I > ()
		{
			std::shared_ptr< OutletImpl > outlet = checkValidity< OutletImpl >( mImpl, "outlet" );
			return std::static_pointer_cast< DataSource_I >( outlet );
		}
	}
}
