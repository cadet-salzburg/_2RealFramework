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

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "inlet handle not initialized" );

namespace _2Real
{
	namespace app
	{
		ParameterHandle::ParameterHandle() :
			m_ParameterIO( nullptr )
		{
		}

		ParameterHandle::ParameterHandle( ParameterIO &paramIO ) :
			m_ParameterIO( &paramIO )
		{
			m_ParameterIO->registerHandle( *this );
		}

		ParameterHandle::~ParameterHandle()
		{
			if ( isValid() ) m_ParameterIO->unregisterHandle( *this );
		}

		ParameterHandle::ParameterHandle( ParameterHandle const& other ) :
			m_ParameterIO( other.m_ParameterIO )
		{
			if ( isValid() ) m_ParameterIO->registerHandle( *this );
		}

		ParameterHandle& ParameterHandle::operator=( ParameterHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_ParameterIO->unregisterHandle( *this );
			}

			m_ParameterIO = other.m_ParameterIO;

			if ( isValid() )
			{
				m_ParameterIO->registerHandle( *this );
			}

			return *this;
		}

		bool ParameterHandle::isValid() const
		{
			return m_ParameterIO != nullptr;
		}

		void ParameterHandle::invalidate()
		{
			m_ParameterIO = nullptr;
		}

		bool ParameterHandle::operator==( ParameterHandle const& other ) const
		{
			return m_ParameterIO == other.m_ParameterIO;
		}

		bool ParameterHandle::operator!=( ParameterHandle const& other ) const
		{
			return m_ParameterIO != other.m_ParameterIO;
		}

		bool ParameterHandle::operator<( ParameterHandle const& other ) const
		{
			return m_ParameterIO < other.m_ParameterIO;
		}

		bool ParameterHandle::operator<=( ParameterHandle const& other ) const
		{
			return m_ParameterIO <= other.m_ParameterIO;
		}

		bool ParameterHandle::operator>( ParameterHandle const& other ) const
		{
			return m_ParameterIO > other.m_ParameterIO;
		}

		bool ParameterHandle::operator>=( ParameterHandle const& other ) const
		{
			return m_ParameterIO >= other.m_ParameterIO;
		}

		std::string const& ParameterHandle::getName() const
		{
			checkValidity( m_ParameterIO );
			return m_ParameterIO->info().baseName;
		}

		app::BlockHandle ParameterHandle::getOwningBlock()
		{
			checkValidity( m_ParameterIO );

			AbstractUberBlock *b = m_ParameterIO->getOwningBlock();
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

		app::TypeMetainfo ParameterHandle::getType() const
		{
			checkValidity( m_ParameterIO );
			return app::TypeMetainfo( m_ParameterIO->info().type );
		}

		std::shared_ptr< CustomType > ParameterHandle::makeData() const
		{
			checkValidity( m_ParameterIO );
			return std::shared_ptr< CustomType >( new CustomType( m_ParameterIO->info().type ) );
		}

		std::shared_ptr< const CustomType > ParameterHandle::getCurrentData() const
		{
			checkValidity( m_ParameterIO );
			return m_ParameterIO->m_Parameter->getDataThreadsafe();
		}

		void ParameterHandle::setData( std::shared_ptr< const CustomType > data )
		{
			checkValidity( m_ParameterIO );
			m_ParameterIO->m_Parameter->setData( data );
		}
	}
}