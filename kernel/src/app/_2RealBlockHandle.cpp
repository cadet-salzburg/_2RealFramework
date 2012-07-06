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
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealBlockData.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealException.h"

using std::string;

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );\

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
			Handle( &block ),
			m_Block( &block )
		{
			m_Block->registerHandle( *this );
		}

		BlockHandle::~BlockHandle()
		{
			if ( isValid() ) m_Block->unregisterHandle( *this );
		}

		BlockHandle::BlockHandle( BlockHandle const& other ) :
			Handle( other.m_Block ),
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

		BlockInfo BlockHandle::getBlockInfo() const
		{
			checkHandle( m_Block );
			return m_Block->getBlockData();
		}

		void BlockHandle::setUpdateRate( const double updatesPerSecond )
		{
			checkHandle( m_Block );
			m_Block->updateWithFixedRate( updatesPerSecond );
		}

		void BlockHandle::setup()
		{
			checkHandle( m_Block );
			m_Block->setUp();
		}

		void BlockHandle::start()
		{
			checkHandle( m_Block );
			m_Block->start();
		}

		void BlockHandle::stop( const long timeout )
		{
			checkHandle( m_Block );
			m_Block->stop( true, timeout );
		}

		InletHandle & BlockHandle::getInletHandle( string const& name ) const
		{
			checkHandle( m_Block );
			return m_Block->getAppInletHandle( name );
		}

		OutletHandle & BlockHandle::getOutletHandle( string const& name ) const
		{
			checkHandle( m_Block );
			return m_Block->getAppOutletHandle( name );
		}

		BlockHandle::InletHandles const& BlockHandle::getAllInletHandles() const
		{
			checkHandle( m_Block );
			return m_Block->getAppInletHandles();
		}

		BlockHandle::OutletHandles const& BlockHandle::getAllOutletHandles() const
		{
			checkHandle( m_Block );
			return m_Block->getAppOutletHandles();
		}

		void BlockHandle::registerToNewData( BlockDataCallback callback, void *userData ) const
		{
			checkHandle( m_Block );
			BlockCallback *cb = new FunctionCallback< std::list< AppData > const& >( callback, userData );
			m_Block->registerToNewData( *cb );
		}

		void BlockHandle::unregisterFromNewData( BlockDataCallback callback, void *userData ) const
		{
			checkHandle( m_Block );
			BlockCallback *cb = new FunctionCallback< std::list< AppData > const& >( callback, userData );
			m_Block->unregisterFromNewData( *cb);
		}

		void BlockHandle::registerToNewDataInternal( BlockCallback &cb ) const
		{
			checkHandle( m_Block );
			m_Block->registerToNewData( cb );
		}

		void BlockHandle::unregisterFromNewDataInternal( BlockCallback &cb ) const
		{
			checkHandle( m_Block );
			m_Block->unregisterFromNewData( cb );
		}

		void BlockHandle::kill( const long timeout )
		{
			checkHandle( m_Block );
			EngineImpl::instance().killBlockInstance( *m_Block, timeout );
		}

		void BlockHandle::singleStep()
		{
			checkHandle( m_Block );
			m_Block->singleStep();
		}
	}
}