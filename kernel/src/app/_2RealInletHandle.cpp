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
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealFunctionBlock.h"

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
		InletHandle::InletHandle() :
			mImpl()
		{
		}

		InletHandle::InletHandle( std::shared_ptr< AbstractInletIO > inlet ) :
			mImpl( inlet )
		{
		}

		bool InletHandle::isValid() const
		{
			std::shared_ptr< AbstractInletIO > inlet = mImpl.lock();
			return ( inlet.get() != nullptr );
		}

		bool InletHandle::link( OutletHandle o )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( o.mImpl, "outlet" );
			return ( inlet->operator[]( 0 ) )->linkTo( outlet );
		}

		void InletHandle::unlinkFrom( OutletHandle o )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( o.mImpl, "outlet" );
			return ( inlet->operator[]( 0 ) )->unlinkFrom( outlet );
		}

		std::string const& InletHandle::getName() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return inlet->getInfo()->name;
		}

		app::TypeMetainfo InletHandle::getType() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return app::TypeMetainfo( inlet->getInfo()->type );
		}

		std::shared_ptr< CustomType > InletHandle::makeData() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return std::shared_ptr< CustomType >( new CustomType( inlet->getInfo()->type ) );
		}

		std::shared_ptr< const CustomType > InletHandle::getCurrentData() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return ( inlet->operator[]( 0 ) )->getCurrentData();
		}

		void InletHandle::receiveData( std::shared_ptr< const CustomType > data )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			( inlet->operator[]( 0 ) )->receiveData( data );
		}

		void InletHandle::setUpdatePolicy( Policy const& p )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			( inlet->operator[]( 0 ) )->setUpdatePolicy( p );
		}

		void InletHandle::setBufferSize( const unsigned int size )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			( inlet->operator[]( 0 ) )->setQueueSize( size );
		}

		bool InletHandle::isMultiInlet() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return inlet->isMultiInlet();
		}

		unsigned int InletHandle::getSize() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return inlet->getSize();
		}

		InletHandle InletHandle::operator[]( const unsigned int index )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return InletHandle( inlet->operator[]( index ) );
		}

		InletHandle InletHandle::add()
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return InletHandle( inlet->addBasicInlet() );
		}

		void InletHandle::remove( InletHandle handle )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			std::shared_ptr< AbstractInletIO > other = checkValidity< AbstractInletIO >( handle.mImpl, "inlet" );
			inlet->removeBasicInlet( other );
		}
	}
}