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

#include "app/_2RealContextBlockHandle.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"

using std::string;

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "context block handle not initialized" );

namespace _2Real
{
	namespace app
	{
		ContextBlockHandle::ContextBlockHandle() :
			m_Block( nullptr )
		{
		}

		ContextBlockHandle::ContextBlockHandle( FunctionBlock< ContextBlockHandle > &block ) :
			m_Block( &block )
		{
			m_Block->registerHandle( *this );
		}

		ContextBlockHandle::~ContextBlockHandle()
		{
			if ( isValid() ) m_Block->unregisterHandle( *this );
		}

		ContextBlockHandle::ContextBlockHandle( ContextBlockHandle const& other ) :
			m_Block( other.m_Block )
		{
			if ( isValid() ) m_Block->registerHandle( *this );
		}

		ContextBlockHandle& ContextBlockHandle::operator=( ContextBlockHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_Block->unregisterHandle( *this );
			}

			m_Block = other.m_Block;

			if ( isValid() )
			{
				m_Block->registerHandle( *this );
			}

			return *this;
		}

		bool ContextBlockHandle::isValid() const
		{
			return m_Block != nullptr;
		}

		bool ContextBlockHandle::operator==( ContextBlockHandle const& other ) const
		{
			return m_Block == other.m_Block;
		}

		bool ContextBlockHandle::operator!=( ContextBlockHandle const& other ) const
		{
			return m_Block != other.m_Block;
		}

		bool ContextBlockHandle::operator<( ContextBlockHandle const& other ) const
		{
			return m_Block < other.m_Block;
		}

		bool ContextBlockHandle::operator<=( ContextBlockHandle const& other ) const
		{
			return m_Block <= other.m_Block;
		}

		bool ContextBlockHandle::operator>( ContextBlockHandle const& other ) const
		{
			return m_Block > other.m_Block;
		}

		bool ContextBlockHandle::operator>=( ContextBlockHandle const& other ) const
		{
			return m_Block >= other.m_Block;
		}

		void ContextBlockHandle::invalidate()
		{
			m_Block = nullptr;
		}

		BlockInfo const& ContextBlockHandle::getBlockInfo() const
		{
			checkValidity( m_Block );
			return m_Block->getBlockInfo();
		}

		OutletHandle & ContextBlockHandle::getOutletHandle( string const& name ) const
		{
			checkValidity( m_Block );
			return m_Block->getAppOutletHandle( toLower( trim( name ) ) );
		}

		ContextBlockHandle::OutletHandles const& ContextBlockHandle::getAllOutletHandles() const
		{
			checkValidity( m_Block );
			return m_Block->getAppOutletHandles();
		}

		void ContextBlockHandle::registerToNewData( BlockDataCallback callback, void *userData ) const
		{
			checkValidity( m_Block );
			BlockCallback *cb = new FunctionCallback< std::list< AppData > const& >( callback, userData );
			m_Block->registerToNewData( *cb );
		}

		void ContextBlockHandle::unregisterFromNewData( BlockDataCallback callback, void *userData ) const
		{
			checkValidity( m_Block );
			BlockCallback *cb = new FunctionCallback< std::list< AppData > const& >( callback, userData );
			m_Block->unregisterFromNewData( *cb);
		}

		void ContextBlockHandle::registerToNewDataInternal( BlockCallback &cb ) const
		{
			checkValidity( m_Block );
			m_Block->registerToNewData( cb );
		}

		void ContextBlockHandle::unregisterFromNewDataInternal( BlockCallback &cb ) const
		{
			checkValidity( m_Block );
			m_Block->unregisterFromNewData( cb );
		}
	}
}