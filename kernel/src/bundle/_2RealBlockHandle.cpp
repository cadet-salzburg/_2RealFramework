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

#include "bundle/_2RealBlockHandle.h"
#include "engine/_2RealFunctionBlockIOManager.h"
#include "helpers/_2RealStringHelpers.h"

using std::string;

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );

namespace _2Real
{
	namespace bundle
	{
		BlockHandle::BlockHandle() :
			m_IO( nullptr )
		{
		}

		BlockHandle::BlockHandle( FunctionBlockIOManager &io ) :
			m_IO( &io )
		{
			m_IO->registerHandle( *this );
		}

		BlockHandle::~BlockHandle()
		{
			if ( isValid() ) m_IO->unregisterHandle( *this );
		}

		BlockHandle::BlockHandle( BlockHandle const& other ) :
			m_IO( other.m_IO )
		{
			if ( isValid() ) m_IO->registerHandle( *this );
		}

		BlockHandle& BlockHandle::operator=( BlockHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_IO->unregisterHandle( *this );
			}

			m_IO = other.m_IO;

			if ( isValid() )
			{
				m_IO->registerHandle( *this );
			}

			return *this;
		}

		bool BlockHandle::isValid() const
		{
			return m_IO != nullptr;
		}

		void BlockHandle::invalidate()
		{
			m_IO = nullptr;
		}

		InletHandle & BlockHandle::getInletHandle( string const& name ) const
		{
			checkValidity( m_IO );
			return m_IO->getBundleInletHandle( trim ( name ) );
		}

		OutletHandle & BlockHandle::getOutletHandle( string const& name ) const
		{
			checkValidity( m_IO );
			return m_IO->getBundleOutletHandle( trim ( name ) );
		}

		BlockHandle::InletHandles const& BlockHandle::getAllInletHandles() const
		{
			checkValidity( m_IO );
			return m_IO->getBundleInletHandles();
		}

		BlockHandle::OutletHandles const& BlockHandle::getAllOutletHandles() const
		{
			checkValidity( m_IO );
			return m_IO->getBundleOutletHandles();
		}
	}
}