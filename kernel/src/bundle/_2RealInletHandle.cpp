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

#include "bundle/_2RealInletHandle.h"
#include "engine/_2RealInlet.h"

namespace _2Real
{
	template< typename TObj >
	std::shared_ptr< TObj > checkValidity( std::weak_ptr< TObj > handle, std::string const& what )
	{
		std::shared_ptr< TObj > locked = handle.lock();
		if ( locked.get() == nullptr )
		{
			std::stringstream msg;
			msg << "nullptr access: " << what << " handle does not point to an object" << std::endl;
		}

		return locked;
	}

	namespace bundle
	{
		InletHandle::InletHandle() :
			mImpl()
		{
		}

		InletHandle::InletHandle( std::shared_ptr< AbstractInlet > inlet ) :
			mImpl( inlet )
		{
		}

		bool InletHandle::isValid() const
		{
			std::shared_ptr< AbstractInlet > inlet = mImpl.lock();
			return ( inlet.get() != nullptr );
		}

		InletHandle InletHandle::operator[]( const unsigned int index )
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return InletHandle( inlet->operator[]( index ) );
		}

		bool InletHandle::hasChanged() const
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return inlet->operator[]( 0 )->hasChanged();
		}

		bool InletHandle::canExpand() const
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return inlet->canExpand();
		}

		unsigned int InletHandle::getSubInletCount() const
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return inlet->getSubInletCount();
		}

		std::shared_ptr< const CustomType > InletHandle::getReadableRef() const
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return inlet->operator[]( 0 )->getData();
		}

		std::shared_ptr< CustomType > InletHandle::getWriteableCopy() const
		{
			std::shared_ptr< AbstractInlet > inlet = checkValidity< AbstractInlet >( mImpl, "inlet" );
			return std::shared_ptr< CustomType >( new CustomType( *( inlet->operator[]( 0 )->getData().get() ) ) );
		}
	}
}