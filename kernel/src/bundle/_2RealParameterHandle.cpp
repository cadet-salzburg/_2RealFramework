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

#include "bundle/_2RealParameterHandle.h"
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
		ParameterHandle::ParameterHandle() :
			mImpl()
		{
		}

		ParameterHandle::ParameterHandle( std::shared_ptr< AbstractInlet > parameter ) :
			mImpl( parameter )
		{
		}

		bool ParameterHandle::isValid() const
		{
			std::shared_ptr< AbstractInlet > parameter = mImpl.lock();
			return ( parameter.get() != nullptr );
		}

		bool ParameterHandle::hasChanged() const
		{
			std::shared_ptr< AbstractInlet > parameter = checkValidity( mImpl, "parameter" );
			return ( parameter->operator[]( 0 ) )->hasChanged();
		}

		std::shared_ptr< const CustomType > ParameterHandle::getReadableRef() const
		{
			std::shared_ptr< AbstractInlet > parameter = checkValidity( mImpl, "parameter" );
			return ( parameter->operator[]( 0 ) )->getData();
		}

		std::shared_ptr< CustomType > ParameterHandle::getWriteableCopy() const
		{
			std::shared_ptr< AbstractInlet > parameter = checkValidity( mImpl, "parameter" );
			return std::shared_ptr< CustomType >( new CustomType( *( parameter->operator[]( 0 ) )->getData().get() ) );
		}
	}
}