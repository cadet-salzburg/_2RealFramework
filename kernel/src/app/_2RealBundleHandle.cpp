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

#include "_2RealBundleHandle.h"
#include "_2RealBundleInternal.h"
#include "_2RealException.h"
#include "_2RealBlockHandle.h"

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		BundleHandle::BundleHandle() :
			m_Bundle( nullptr )
		{
		}

		BundleHandle::BundleHandle( BundleInternal &bundle ) :
			m_Bundle( &bundle )
		{
		}

		BundleData const& BundleHandle::getBundleData() const
		{
			checkHandle( m_Bundle );
			return m_Bundle->getMetadata();
		}

		BlockHandle BundleHandle::createBlockInstance( std::string const& blockName )
		{
			checkHandle( m_Bundle );
			return m_Bundle->createBlockInstance( blockName );
		}
	}
}