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

#include "_2RealContextBlockHandle.h"
#include "_2RealFunctionBlock.h"
#include "_2RealException.h"
#include "_2RealUpdatePolicyHandle.h"

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		ContextBlockHandle::ContextBlockHandle() :
			m_Block( nullptr )
		{
		}

		ContextBlockHandle::ContextBlockHandle( FunctionBlock &block ) :
			m_Block( &block )
		{
		}

		BlockData const& ContextBlockHandle::getBlockData() const
		{
			checkHandle( m_Block );
			return m_Block->getMetadata();
		}

		void ContextBlockHandle::setUpdateRate( const double updatesPerSecond )
		{
			checkHandle( m_Block );
			UpdatePolicyHandle updatePolicy = m_Block->getUpdatePolicyHandle();
			updatePolicy.updateWithFixedRate( updatesPerSecond );
		}

		void ContextBlockHandle::setUp()
		{
			checkHandle( m_Block );
			m_Block->setUp();
		}

		void ContextBlockHandle::start()
		{
			checkHandle( m_Block );
			m_Block->start();
		}
	}
}