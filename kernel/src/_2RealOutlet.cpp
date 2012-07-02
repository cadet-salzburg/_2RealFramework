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

#include "_2RealOutlet.h"
#include "_2RealInlet.h"
#include "_2RealInletBuffer.h"
#include "_2RealException.h"
#include "_2RealAbstractUberBlock.h"
#include "_2RealEngineImpl.h"
#include "app/_2RealInletHandle.h"

#ifdef _DEBUG
	#include <assert.h>
#endif

using std::string;
using std::find;
using std::ostringstream;

namespace _2Real
{

	Outlet::Outlet( AbstractUberBlock &owner, string const& name, string const& longTypename, string const& typeName, EngineData const& emptyData ) :
		Parameter( owner, name, longTypename, typeName ),
		m_Engine( EngineImpl::instance() ),
		m_WriteDataItem( emptyData, 0 ),
		m_DiscardCurrent( false )
	{
		Parameter::setData( TimestampedData( emptyData, 0 ) );
		Parameter::synchronize();
	}

	void Outlet::update()
	{
		if ( !m_DiscardCurrent )
		{
			TimestampedData data( m_WriteDataItem.getData(), m_Engine.getElapsedTime() );

			// copy data back into the outlet's writing data
			m_WriteDataItem.cloneData( data );

			// inlets get the datat that was really written, not a copy
			m_InletEvent.notify( this, data );
			
			Parameter::setData( data );
			Parameter::synchronize();		// immediately makes data visible to the app
		}
	}

	EngineData & Outlet::getDataForWriting()
	{
		return m_WriteDataItem.getData();
	}

	void Outlet::discardCurrentUpdate()
	{
		m_DiscardCurrent = true;
	}

	void Outlet::linkTo( app::InletHandle &inletHandle )
	{
		Param *p = inletHandle.m_Param;
#ifdef _DEBUG
		if ( p == nullptr ) assert( NULL );
#endif

		Inlet &inlet = dynamic_cast< Inlet & >( *p );
		Param::m_Owner.createLink( inlet, *this );
	}

	void Outlet::unlinkFrom( app::InletHandle &inletHandle )
	{
		Param *p = inletHandle.m_Param;
#ifdef _DEBUG
		if ( p == nullptr ) assert( NULL );
#endif

		Inlet &inlet = dynamic_cast< Inlet & >( *p );
		Param::m_Owner.createLink( inlet, *this );
	}

	void Outlet::addInletListener( InletBuffer &buffer )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		m_InletEvent += Poco::delegate( &buffer, &InletBuffer::receiveData );
	}

	void Outlet::removeInletListener( InletBuffer &buffer )
	{
		Poco::FastMutex::ScopedLock lock( m_Access );
		m_InletEvent -= Poco::delegate( &buffer, &InletBuffer::receiveData );
	}

	void Outlet::registerToNewData( app::OutletDataCallback callback, void *userData )
	{
		Param::m_Owner.registerToNewData( *this, callback, userData );
	}

	void Outlet::unregisterFromNewData( app::OutletDataCallback callback, void *userData )
	{
		Param::m_Owner.unregisterFromNewData( *this, callback, userData );
	}

	void Outlet::registerToNewData( app::AbstractOutletDataCallbackHandler &handler )
	{
		Param::m_Owner.registerToNewData( *this, handler );
	}

	void Outlet::unregisterFromNewData( app::AbstractOutletDataCallbackHandler &handler )
	{
		Param::m_Owner.unregisterFromNewData( *this, handler );
	}
}