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
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealParameter.h"

namespace _2Real
{
	namespace app
	{
		ParameterHandle::ParameterHandle() :
			mImpl()
		{
		}

		ParameterHandle::ParameterHandle( std::shared_ptr< Parameter > inlet ) :
			mImpl( inlet )
		{
		}

		bool ParameterHandle::isValid() const
		{
			std::shared_ptr< Parameter > param = mImpl.lock();
			return ( nullptr != param.get() );
		}

		std::string ParameterHandle::getName() const
		{
			std::shared_ptr< Parameter > parameter = checkValidity< Parameter >( mImpl, "parameter" );
			return parameter->getName();
		}

		//DataItem ParameterHandle::getValue() const
		//{
		//	std::shared_ptr< Parameter > parameter = checkValidity< Parameter >( mImpl, "parameter" );
		//	return *( parameter->getTmpValue().get() );
		//}

		//DataItem ParameterHandle::getCurrentValue() const
		//{
		//	std::shared_ptr< Parameter > parameter = checkValidity< Parameter >( mImpl, "parameter" );
		//	return parameter->getValue();
		//}

		//void ParameterHandle::setValue( DataItem && value )
		//{
		//	std::shared_ptr< Parameter > parameter = checkValidity< Parameter >( mImpl, "parameter" );
		//	parameter->setTmpValue( std::move( value ) );
		//}
	}
}