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

#include "app/_2RealInletHandle.h"
#include "app/_2RealLinkHandle.h"
#include "app/_2RealTypeMetainfo.h"
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealInlet.h"

namespace _2Real
{
	namespace app
	{
		InletHandle::InletHandle() :
			AbstractInletHandle(),
			mImpl()
		{
		}

		InletHandle::InletHandle( std::shared_ptr< Inlet > inlet ) :
			AbstractInletHandle( inlet ),
			mImpl( inlet )
		{
		}

		//void InletHandle::setValue( DataItem && value )
		//{
		//	std::shared_ptr< Inlet > inlet = checkValidity< Inlet >( mImpl, "inlet" );
		//	inlet->setTmpValue( std::move( value ) );
		//}

		//DataItem InletHandle::getValue() const
		//{
		//	std::shared_ptr< Inlet > inlet = checkValidity< Inlet >( mImpl, "inlet" );
		//	return *( inlet->getTmpValue().get() );
		//}

		//DataItem InletHandle::getCurrentValue() const
		//{
		//	std::shared_ptr< Inlet > inlet = checkValidity< Inlet >( mImpl, "inlet" );
		//	return inlet->getValue();
		//}

		InletHandle::operator std::shared_ptr< DataSink > ()
		{
			std::shared_ptr< Inlet > inlet = checkValidity< Inlet >( mImpl, "inlet" );
			return std::static_pointer_cast< DataSink >( inlet );
		}

		LinkHandle InletHandle::linkTo( std::shared_ptr< DataSource > source )
		{
			std::shared_ptr< Inlet > inlet = checkValidity< Inlet >( mImpl, "inlet" );
			return LinkHandle( inlet->linkTo( source ) );
		}

		TypeMetainfo InletHandle::getTypeMetainfo() const
		{
			std::shared_ptr< Inlet > inlet = checkValidity< Inlet >( mImpl, "inlet" );
			return TypeMetainfo( inlet->getTypeMetainfo() );
		}
	}
}