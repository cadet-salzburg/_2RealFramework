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

#include "bundle/_2RealMultiInletHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "engine/_2RealMultiInlet.h"

namespace _2Real
{
	namespace bundle
	{
		MultiInletHandle::MultiInletHandle( std::shared_ptr< MultiInlet > inlet ) :
			AbstractInletHandle( inlet ),
			mImpl( inlet )
		{
		}
	
		uint32_t MultiInletHandle::getSize() const
		{
			return mImpl->getSize();
		}

		bool MultiInletHandle::isEmpty() const
		{
			return mImpl->isEmpty();
		}

		//InletHandle MultiInletHandle::operator[]( const uint32_t id )
		//{
		//	return InletHandle( mImpl->operator[]( id ) );
		//}
	}
}