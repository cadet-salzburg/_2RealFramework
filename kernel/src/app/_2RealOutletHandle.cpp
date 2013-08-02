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
#include "app/_2RealOutletHandle.h"
#include "app/_2RealInfo.h"
#include "app/_2RealLinkHandle.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealIOMetadata.h"

namespace _2Real
{
	namespace app
	{
		OutletHandle::OutletHandle() :
			mImpl()
		{
		}

		OutletHandle::OutletHandle( std::shared_ptr< OutletIO > outlet ) :
			mImpl( outlet )
		{
		}

		bool OutletHandle::isValid() const
		{
			std::shared_ptr< OutletIO > outlet = mImpl.lock();
			return ( outlet.get() != nullptr );
		}

		LinkHandle OutletHandle::link( InletHandle i )
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( i.mImpl, "inlet" );
			return LinkHandle( outlet->linkTo( inlet->operator[]( 0 ) ) );
		}

		void OutletHandle::unlinkFrom( InletHandle i )
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( i.mImpl, "inlet" );
			return outlet->unlinkFrom( inlet->operator[]( 0 ) );
		}

		void OutletHandle::registerToNewData( OutletDataCallback callback, void *userData ) const
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			OutletCallback *cb = new FunctionCallback< std::shared_ptr< const CustomType > >( callback, userData );
			outlet->mAppEvent->addListener( *cb );
		}

		void OutletHandle::unregisterFromNewData( OutletDataCallback callback, void *userData ) const
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			OutletCallback *cb = new FunctionCallback< std::shared_ptr< const CustomType > >( callback, userData );
			outlet->mAppEvent->removeListener( *cb );
		}

		void OutletHandle::registerToNewDataInternal( OutletCallback &cb ) const
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			outlet->mAppEvent->addListener( cb );
		}

		void OutletHandle::unregisterFromNewDataInternal( OutletCallback &cb ) const
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			outlet->mAppEvent->removeListener( cb );
		}

		std::string const& OutletHandle::getName() const
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			return outlet->getInfo()->name;
		}

		TypeMetainfo OutletHandle::getType() const
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			return TypeMetainfo( outlet->getInfo()->typeMetadata );
		}

		std::shared_ptr< CustomType > OutletHandle::makeData() const
		{
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( mImpl, "outlet" );
			return std::shared_ptr< CustomType >( new CustomType( outlet->getInfo()->typeMetadata ) );
		}
	}
}