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
#include "app/_2RealBlockHandle.h"
#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealAbstractIOManager.h"

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

namespace _2Real
{
	namespace app
	{
		ParameterHandle::ParameterHandle() :
			mImpl()
		{
		}

		ParameterHandle::ParameterHandle( std::shared_ptr< ParameterIO > parameter ) :
			mImpl( parameter )
		{
		}

		bool ParameterHandle::isValid() const
		{
			std::shared_ptr< ParameterIO > parameter = mImpl.lock();
			return ( parameter.get() == nullptr );
		}

		std::string const& ParameterHandle::getName() const
		{
			std::shared_ptr< ParameterIO > parameter = checkValidity< ParameterIO >( mImpl, "parameter" );
			return parameter->getInfo()->name;
		}

		app::TypeMetainfo ParameterHandle::getType() const
		{
			std::shared_ptr< ParameterIO > parameter = checkValidity< ParameterIO >( mImpl, "parameter" );
			return app::TypeMetainfo( parameter->getInfo()->type );
		}

		std::shared_ptr< CustomType > ParameterHandle::makeData() const
		{
			std::shared_ptr< ParameterIO > parameter = checkValidity< ParameterIO >( mImpl, "parameter" );
			return std::shared_ptr< CustomType >( new CustomType( parameter->getInfo()->type ) );
		}

		std::shared_ptr< const CustomType > ParameterHandle::getCurrentData() const
		{
			std::shared_ptr< ParameterIO > parameter = checkValidity< ParameterIO >( mImpl, "parameter" );
			return parameter->getCurrentDataThreadsafe();
		}

		void ParameterHandle::setData( std::shared_ptr< const CustomType > data )
		{
			std::shared_ptr< ParameterIO > parameter = checkValidity< ParameterIO >( mImpl, "parameter" );
			parameter->setData( data );
		}
	}
}