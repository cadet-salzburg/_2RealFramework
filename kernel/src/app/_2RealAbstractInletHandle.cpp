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

#include "app/_2RealAbstractInletHandle.h"
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealAbstractInlet.h"

namespace _2Real
{
	namespace app
	{
		AbstractInletHandle::AbstractInletHandle() :
			mImpl()
		{
		}

		AbstractInletHandle::AbstractInletHandle( std::shared_ptr< AbstractInlet > inlet ) :
			mImpl( inlet )
		{
		}

		AbstractInletHandle::~AbstractInletHandle()
		{
		}

		bool AbstractInletHandle::isValid() const
		{
			std::shared_ptr< AbstractInlet > inlet = mImpl.lock();
			return ( nullptr != inlet.get() );
		}

		std::string AbstractInletHandle::getName() const
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return inlet->getName();
		}

		bool AbstractInletHandle::isMultiInlet() const
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return inlet->isMultiInlet();
		}
	}
}