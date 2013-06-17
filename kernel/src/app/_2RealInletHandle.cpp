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
#include "app/_2RealParameterInfo.h"
//#include "app/_2RealAppData.h"
#include "engine/_2RealEngineImpl.h"
//#include "engine/_2RealInlet.h"
//#include "engine/_2RealInletBuffer.h"
//#include "engine/_2RealTimestampedData.h"
//#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealFunctionBlock.h"

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "inlet handle not initialized" );

namespace _2Real
{
	namespace app
	{
		InletHandle::InletHandle() :
			m_InletIO( nullptr )
		{
		}

		InletHandle::InletHandle( AbstractInletIO &inletIO ) :
			m_InletIO( &inletIO )
		{
			m_InletIO->registerHandle( *this );
		}

		InletHandle::~InletHandle()
		{
			if ( isValid() ) m_InletIO->unregisterHandle( *this );
		}

		InletHandle::InletHandle( InletHandle const& other ) :
			m_InletIO( other.m_InletIO )
		{
			if ( isValid() ) m_InletIO->registerHandle( *this );
		}

		InletHandle& InletHandle::operator=( InletHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_InletIO->unregisterHandle( *this );
			}

			m_InletIO = other.m_InletIO;

			if ( isValid() )
			{
				m_InletIO->registerHandle( *this );
			}

			return *this;
		}

		bool InletHandle::isValid() const
		{
			return m_InletIO != nullptr;
		}

		void InletHandle::invalidate()
		{
			m_InletIO = nullptr;
		}

		bool InletHandle::operator==( InletHandle const& other ) const
		{
			return m_InletIO == other.m_InletIO;
		}

		bool InletHandle::operator!=( InletHandle const& other ) const
		{
			return m_InletIO != other.m_InletIO;
		}

		bool InletHandle::operator<( InletHandle const& other ) const
		{
			return m_InletIO < other.m_InletIO;
		}

		bool InletHandle::operator<=( InletHandle const& other ) const
		{
			return m_InletIO <= other.m_InletIO;
		}

		bool InletHandle::operator>( InletHandle const& other ) const
		{
			return m_InletIO > other.m_InletIO;
		}

		bool InletHandle::operator>=( InletHandle const& other ) const
		{
			return m_InletIO >= other.m_InletIO;
		}

		bool InletHandle::link( OutletHandle &outlet )
		{
			checkValidity( m_InletIO );
			return EngineImpl::instance().createLink( ( *m_InletIO )[ 0 ], *( outlet.m_OutletIO ) ).isValid();
		}

		void InletHandle::unlinkFrom( OutletHandle &outlet )
		{
			checkValidity( m_InletIO );
			EngineImpl::instance().destroyLink( ( *m_InletIO )[ 0 ], *( outlet.m_OutletIO ) );
		}

		std::string const& InletHandle::getName() const
		{
			checkValidity( m_InletIO );
			return m_InletIO->info().baseName;
		}

		app::BlockHandle InletHandle::getOwningBlock()
		{
			checkValidity( m_InletIO );

			AbstractUberBlock *b = m_InletIO->getOwningBlock();
			FunctionBlock< BlockHandle > *f = dynamic_cast< FunctionBlock< BlockHandle > * >( b );
			if ( nullptr != f )
			{
				return f->getHandle();
			}
			else
			{
				return app::BlockHandle();
			}
		}

		app::TypeMetainfo InletHandle::getType() const
		{
			checkValidity( m_InletIO );
			return app::TypeMetainfo( m_InletIO->info().type );
		}

		std::shared_ptr< const CustomType > InletHandle::getCurrentData() const
		{
			checkValidity( m_InletIO );
			return ( *m_InletIO )[ 0 ].getCurrentData();
		}

		//std::shared_ptr< const CustomType > InletHandle::getCurrentData() const
		//{
		//}

		void InletHandle::setUpdatePolicy( InletPolicy const& p )
		{
			checkValidity( m_InletIO );
			( *m_InletIO )[ 0 ].setUpdatePolicy( p );
		}

		void InletHandle::setBufferSize( const unsigned int size )
		{
			checkValidity( m_InletIO );
			( *m_InletIO )[ 0 ].setBufferSize( size );
		}

		bool InletHandle::isMultiInlet() const
		{
			checkValidity( m_InletIO );
			return m_InletIO->isMultiInlet();
		}

		unsigned int InletHandle::getSize() const
		{
			checkValidity( m_InletIO );
			return m_InletIO->getSize();
		}

		InletHandle InletHandle::operator[]( const unsigned int index )
		{
			checkValidity( m_InletIO );
			return ( *m_InletIO )[ index ].getHandle();
		}

		InletHandle InletHandle::add()
		{
			checkValidity( m_InletIO );
			return m_InletIO->addBasicInlet()->getHandle();
		}

		void InletHandle::remove( InletHandle &handle )
		{
			checkValidity( m_InletIO );

			if ( handle.isValid() )
			{
				// TODO: check if this handle actually belongs to the inlet
				m_InletIO->removeBasicInlet( handle.m_InletIO );
			}
		}
	}
}