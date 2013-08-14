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
#include "app/_2RealInletMetainfo.h"
#include "app/_2RealTypeMetainfo.h"
#include "app/_2RealLinkHandle.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealIOMetadata.h"

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

		LinkHandle InletHandle::link( OutletHandle o )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( o.mImpl, "outlet" );
			return LinkHandle( ( inlet->operator[]( 0 ) )->linkTo( outlet ) );
		}

		void InletHandle::unlinkFrom( OutletHandle o )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			std::shared_ptr< OutletIO > outlet = checkValidity< OutletIO >( o.mImpl, "outlet" );
			return ( inlet->operator[]( 0 ) )->unlinkFrom( outlet );
		}

		InputMetainfo InletHandle::getInletMetainfo() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return InletMetainfo( inlet->getMetadata() );
		}

		TypeMetainfo InletHandle::getTypeMetainfo() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return TypeMetainfo( inlet->getMetadata()->typeMetadata );
		}

		std::shared_ptr< CustomType > InletHandle::makeData() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return std::shared_ptr< CustomType >( new CustomType( inlet->getMetadata()->typeMetadata ) );
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

		void InletHandle::setData( std::shared_ptr< const CustomType > data )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			( inlet->operator[]( 0 ) )->setData( data );
		}

		//void InletHandle::setUpdatePolicy( UpdatePolicy const& p )
		//{
		//	std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
		//	( inlet->operator[]( 0 ) )->setUpdatePolicy( p );
		//}

		//void InletHandle::setBufferSize( const unsigned int size )
		//{
		//	std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
		//	( inlet->operator[]( 0 ) )->setQueueSize( size );
		//}

		bool InletHandle::canExpand() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return inlet->canExpand();
		}

		unsigned int InletHandle::getSubInletCount() const
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return inlet->getSubInletCount();
		}

		InletHandle InletHandle::operator[]( const unsigned int index )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return InletHandle( inlet->operator[]( index ) );
		}

		InletHandle InletHandle::add()
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			return InletHandle( inlet->addSubInlet() );
		}

		void InletHandle::remove( InletHandle handle )
		{
			std::shared_ptr< AbstractInletIO > inlet = checkValidity< AbstractInletIO >( mImpl, "inlet" );
			std::shared_ptr< AbstractInletIO > other = checkValidity< AbstractInletIO >( handle.mImpl, "inlet" );
			inlet->removeSubInlet( other );
		}
	}
}