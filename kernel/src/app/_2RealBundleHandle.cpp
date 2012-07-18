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

#include "app/_2RealBundleHandle.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealBundleInfo.h"
#include "engine/_2RealBundle.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"

#define checkValidity( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "bundle handle not initialized" );

namespace _2Real
{
	namespace app
	{
		BundleHandle::BundleHandle() :
			Handle(),
			m_Bundle( nullptr )
		{
		}

		BundleHandle::BundleHandle( Bundle &bundle ) :
			Handle(),
			m_Bundle( &bundle )
		{
			m_Bundle->registerHandle( *this );
		}

		BundleHandle::~BundleHandle()
		{
			if ( isValid() ) m_Bundle->unregisterHandle( *this );
		}

		BundleHandle::BundleHandle( BundleHandle const& other ) :
			Handle(),
			m_Bundle( other.m_Bundle )
		{
			if ( isValid() ) m_Bundle->registerHandle( *this );
		}

		BundleHandle& BundleHandle::operator=( BundleHandle const& other )
		{
			if ( this == &other )
			{
				return *this;
			}

			if ( isValid() )
			{
				m_Bundle->unregisterHandle( *this );
			}

			Handle::operator=( other );
			m_Bundle = other.m_Bundle;

			if ( isValid() )
			{
				m_Bundle->registerHandle( *this );
			}

			return *this;
		}

		bool BundleHandle::isValid() const
		{
			return m_Bundle != nullptr;
		}

		bool BundleHandle::operator==( BundleHandle const& other ) const
		{
			return m_Bundle == other.m_Bundle;
		}

		bool BundleHandle::operator!=( BundleHandle const& other ) const
		{
			return m_Bundle != other.m_Bundle;
		}

		bool BundleHandle::operator<( BundleHandle const& other ) const
		{
			return m_Bundle < other.m_Bundle;
		}

		bool BundleHandle::operator<=( BundleHandle const& other ) const
		{
			return m_Bundle <= other.m_Bundle;
		}

		bool BundleHandle::operator>( BundleHandle const& other ) const
		{
			return m_Bundle > other.m_Bundle;
		}

		bool BundleHandle::operator>=( BundleHandle const& other ) const
		{
			return m_Bundle >= other.m_Bundle;
		}

		BundleInfo const& BundleHandle::getBundleInfo() const
		{
			checkValidity( m_Bundle );
			return m_Bundle->getBundleInfo();
		}

		ContextBlockHandle & BundleHandle::getContextBlock() const
		{
			checkValidity( m_Bundle );
			return m_Bundle->getContextBlockHandle();
		}

		BlockHandle & BundleHandle::createBlockInstance( std::string const& blockName )
		{
			checkValidity( m_Bundle );
			return m_Bundle->createBlockInstance( toLower( trim( blockName ) ) );
		}

		void BundleHandle::invalidate()
		{
			m_Bundle = nullptr;
		}

		void BundleHandle::unload( const long blockTimeout )
		{
			checkValidity( m_Bundle );
			m_Bundle->unload( blockTimeout );
		}
	}
}