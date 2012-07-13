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

#include "app/_2RealBlockHandle.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );

using std::string;

namespace _2Real
{
	namespace app
	{
		BlockHandle::BlockHandle() :
			Handle(),
			m_Block( nullptr )
		{
		}

		BlockHandle::BlockHandle( FunctionBlock< BlockHandle > &block ) :
			Handle(),
			m_Block( &block )
		{
			m_Block->registerHandle( *this );
		}

		BlockHandle::~BlockHandle()
		{
			if ( isValid() ) m_Block->unregisterHandle( *this );
		}

		BlockHandle::BlockHandle( BlockHandle const& other ) :
			Handle(),
			m_Block( other.m_Block )
		{
			if ( isValid() ) m_Block->registerHandle( *this );
		}

		BlockHandle& BlockHandle::operator=( BlockHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_Block->unregisterHandle( *this );
			}

			Handle::operator=( other );
			m_Block = other.m_Block;

			if ( isValid() )
			{
				m_Block->registerHandle( *this );
			}

			return *this;
		}

		bool BlockHandle::isValid() const
		{
			return m_Block != nullptr;
		}

		bool BlockHandle::operator==( BlockHandle const& other ) const
		{
			return m_Block == other.m_Block;
		}

		bool BlockHandle::operator!=( BlockHandle const& other ) const
		{
			return m_Block != other.m_Block;
		}

		bool BlockHandle::operator<( BlockHandle const& other ) const
		{
			return m_Block < other.m_Block;
		}

		bool BlockHandle::operator<=( BlockHandle const& other ) const
		{
			return m_Block <= other.m_Block;
		}

		bool BlockHandle::operator>( BlockHandle const& other ) const
		{
			return m_Block > other.m_Block;
		}

		bool BlockHandle::operator>=( BlockHandle const& other ) const
		{
			return m_Block >= other.m_Block;
		}

		BlockInfo const& BlockHandle::getBlockInfo() const
		{
			checkValidity( m_Block );
			return m_Block->getBlockInfo();
		}

		void BlockHandle::setUpdateRate( const double updatesPerSecond )
		{
			checkValidity( m_Block );
			m_Block->updateWithFixedRate( updatesPerSecond );
		}

		void BlockHandle::setup()
		{
			checkValidity( m_Block );
			m_Block->setUp();
		}

		void BlockHandle::start()
		{
			checkValidity( m_Block );
			m_Block->start();
		}

		void BlockHandle::stop( const long timeout )
		{
			checkValidity( m_Block );
			m_Block->stop( true, timeout );
		}

		InletHandle & BlockHandle::getInletHandle( string const& name ) const
		{
			checkValidity( m_Block );
			return m_Block->getAppInletHandle( toLower( trim( name ) ) );
		}

		OutletHandle & BlockHandle::getOutletHandle( string const& name ) const
		{
			checkValidity( m_Block );
			return m_Block->getAppOutletHandle( toLower( trim( name ) ) );
		}

		BlockHandle::InletHandles const& BlockHandle::getAllInletHandles() const
		{
			checkValidity( m_Block );
			return m_Block->getAppInletHandles();
		}

		BlockHandle::OutletHandles const& BlockHandle::getAllOutletHandles() const
		{
			checkValidity( m_Block );
			return m_Block->getAppOutletHandles();
		}

		void BlockHandle::registerToNewData( BlockDataCallback callback, void *userData ) const
		{
			checkValidity( m_Block );
			BlockCallback *cb = new FunctionCallback< std::list< AppData > const& >( callback, userData );
			m_Block->registerToNewData( *cb );
		}

		void BlockHandle::unregisterFromNewData( BlockDataCallback callback, void *userData ) const
		{
			checkValidity( m_Block );
			BlockCallback *cb = new FunctionCallback< std::list< AppData > const& >( callback, userData );
			m_Block->unregisterFromNewData( *cb);
		}

		void BlockHandle::registerToNewDataInternal( BlockCallback &cb ) const
		{
			checkValidity( m_Block );
			m_Block->registerToNewData( cb );
		}

		void BlockHandle::unregisterFromNewDataInternal( BlockCallback &cb ) const
		{
			checkValidity( m_Block );
			m_Block->unregisterFromNewData( cb );
		}

		void BlockHandle::kill( const long timeout )
		{
			checkValidity( m_Block );
			EngineImpl::instance().killBlockInstance( *m_Block, timeout );
		}

		void BlockHandle::singleStep()
		{
			checkValidity( m_Block );
			m_Block->singleStep();
		}

		void BlockHandle::invalidate()
		{
			m_Block = nullptr;
		}
	}
}