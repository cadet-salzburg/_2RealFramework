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

#include "app/_2RealParameterHandle.h"
#include "app/_2RealParameterMetainfo.h"
#include "app/_2RealTypeMetainfo.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealIOMetadata.h"

namespace _2Real
{
	namespace app
	{
		ParameterHandle::ParameterHandle() :
			mImpl()
		{
		}

		ParameterHandle::ParameterHandle( std::shared_ptr< AbstractInletIO > parameter ) :
			mImpl( parameter )
		{
		}

		bool ParameterHandle::isValid() const
		{
			std::shared_ptr< AbstractInletIO > parameter = mImpl.lock();
			return ( parameter.get() == nullptr );
		}

		InputMetainfo ParameterHandle::getParameterMetainfo() const
		{
			std::shared_ptr< AbstractInletIO > parameter = checkValidity< AbstractInletIO >( mImpl, "parameter" );
			return ParameterMetainfo( parameter->getMetadata() );
		}

		TypeMetainfo ParameterHandle::getTypeMetainfo() const
		{
			std::shared_ptr< AbstractInletIO > parameter = checkValidity< AbstractInletIO >( mImpl, "parameter" );
			return TypeMetainfo( parameter->getMetadata()->typeMetadata );
		}

		std::shared_ptr< CustomType > ParameterHandle::makeData() const
		{
			std::shared_ptr< AbstractInletIO > parameter = checkValidity< AbstractInletIO >( mImpl, "parameter" );
			return std::shared_ptr< CustomType >( new CustomType( parameter->getMetadata()->typeMetadata ) );
		}

		std::shared_ptr< const CustomType > ParameterHandle::getCurrentData() const
		{
			std::shared_ptr< AbstractInletIO > parameter = checkValidity< AbstractInletIO >( mImpl, "parameter" );
			return ( parameter->operator[]( 0 ) )->getCurrentData();
		}

		void ParameterHandle::setData( std::shared_ptr< const CustomType > data )
		{
			std::shared_ptr< AbstractInletIO > parameter = checkValidity< AbstractInletIO >( mImpl, "parameter" );
			( parameter->operator[]( 0 ) )->setData( data );
		}
	}
}