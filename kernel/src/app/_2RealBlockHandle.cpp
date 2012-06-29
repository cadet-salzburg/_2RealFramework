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

#include "_2RealBlockHandle.h"
#include "_2RealFunctionBlock.h"
#include "_2RealException.h"
#include "_2RealUpdatePolicyHandle.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"
#include "_2RealParameterHandle.h"

using std::string;

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		BlockHandle::BlockHandle() :
			m_Block( nullptr )
		{
		}

		BlockHandle::BlockHandle( FunctionBlock &block ) :
			m_Block( &block )
		{
		}

		BlockData const& BlockHandle::getBlockData() const
		{
			checkHandle( m_Block );
			return m_Block->getMetadata();
		}

		void BlockHandle::setUpdateRate( const double updatesPerSecond )
		{
			checkHandle( m_Block );
			UpdatePolicyHandle updatePolicy = m_Block->getUpdatePolicyHandle();
			updatePolicy.updateWithFixedRate( updatesPerSecond );
		}

		void BlockHandle::setInletUpdatePolicy( const BlockHandle::InletUpdatePolicy p )
		{
			UpdatePolicyHandle updatePolicy = m_Block->getUpdatePolicyHandle();
			if ( p == BlockHandle::ALL_DATA_NEW )
			{
				updatePolicy.updateWhenAllInletDataNew();
			}
			else if ( p == BlockHandle::ALL_DATA_VALID )
			{
				updatePolicy.updateWhenAllInletDataValid();
			}
		}

		void BlockHandle::setUp()
		{
			checkHandle( m_Block );
			m_Block->setUp();
		}

		void BlockHandle::start()
		{
			checkHandle( m_Block );
			m_Block->start();
		}

		void BlockHandle::stop()
		{
			checkHandle( m_Block );
			m_Block->stop( true, 5000 );
		}

		//void BlockHandle::destroy()
		//{
		//	checkHandle( m_Block );
		//}

		//void BlockHandle::singleStep()
		//{
		//	checkHandle( m_Block );
		//}

		InletHandle BlockHandle::getInletHandle( string const& name )
		{
			checkHandle( m_Block );
			return m_Block->createAppInletHandle( name );
		}

		OutletHandle BlockHandle::getOutletHandle( string const& name )
		{
			checkHandle( m_Block );
			return m_Block->createAppOutletHandle( name );
		}

		ParameterHandle BlockHandle::getParameterHandle( string const& name )
		{
			checkHandle( m_Block );
			return m_Block->createAppParameterHandle( name );
		}

		void BlockHandle::registerToNewData( BlockDataCallback callback, void *userData )
		{
			m_Block->registerToNewData( callback, userData );
		}

		void BlockHandle::unregisterFromNewData( BlockDataCallback callback, void *userData )
		{
			m_Block->unregisterFromNewData( callback, userData );
		}

		void BlockHandle::registerToNewDataInternal( AbstractBlockDataCallbackHandler &handler )
		{
			m_Block->registerToNewData( handler );
		}

		void BlockHandle::unregisterFromNewDataInternal( AbstractBlockDataCallbackHandler &handler )
		{
			m_Block->unregisterFromNewData( handler );
		}
	}
}