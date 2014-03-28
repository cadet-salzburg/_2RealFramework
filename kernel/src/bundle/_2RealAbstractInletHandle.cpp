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

#include "bundle/_2RealAbstractInletHandle.h"
#include "engine/_2RealAbstractInlet.h"

namespace _2Real
{
	namespace bundle
	{

		AbstractInletHandle::~AbstractInletHandle()
		{
		}

		AbstractInletHandle::AbstractInletHandle( std::shared_ptr< AbstractInlet > inlet ) :
			mImpl( inlet )
		{
		}

		std::string const& AbstractInletHandle::getName() const
		{
			return mImpl->getName();
		}

		bool AbstractInletHandle::isMultiInlet() const
		{
			assert( mImpl.get() );
			return mImpl->isMultiInlet();
		}
	}
}