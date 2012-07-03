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

#include "engine/_2RealOutlet.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealInletBuffer.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealEngineImpl.h"
#include "app/_2RealInletHandle.h"

#ifdef _DEBUG
	#include <assert.h>
#endif

using std::string;
using std::ostringstream;

namespace _2Real
{

	Outlet::Outlet( AbstractUberBlock &owner, string const& name, string const& longTypename, string const& typeName, EngineData const& emptyData ) :
		HandleAble< app::OutletHandle >( *this ),
		HandleAble< bundle::OutletHandle >( *this ),
		Parameter( owner, name, longTypename, typeName ),
		m_Engine( EngineImpl::instance() ),
		m_WriteDataItem( emptyData, 0 ),
		m_DiscardCurrent( false )
	{
		Parameter::setData( TimestampedData( emptyData, 0 ) );
		Parameter::synchronize();
	}

	bool Outlet::update()
	{
		if ( !m_DiscardCurrent )
		{
			TimestampedData data( m_WriteDataItem.getData(), m_Engine.getElapsedTime() );

			// copy data back into the outlet's writing data
			m_WriteDataItem.cloneData( data );

			m_InletEvent.notify( data );

			Parameter::setData( data );
			Parameter::synchronize();

			return false;
		}
		else
		{
			m_DiscardCurrent = false;
			return true;
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
		Parameter *p = inletHandle.m_Parameter;
#ifdef _DEBUG
		if ( p == nullptr ) assert( NULL );
#endif

		Inlet &inlet = static_cast< Inlet & >( *p );
		Parameter::getOwningUberBlock().createLink( inlet, *this );
	}

	void Outlet::unlinkFrom( app::InletHandle &inletHandle )
	{
		Parameter *p = inletHandle.m_Parameter;
#ifdef _DEBUG
		if ( p == nullptr ) assert( NULL );
#endif

		Inlet &inlet = static_cast< Inlet & >( *p );
		Parameter::getOwningUberBlock().createLink( inlet, *this );
	}

	void Outlet::registerToNewData( app::OutletCallback &callback )
	{
		Parameter::getOwningUberBlock().registerToNewData( *this, callback );
	}

	void Outlet::unregisterFromNewData( app::OutletCallback &callback )
	{
		Parameter::getOwningUberBlock().unregisterFromNewData( *this, callback );
	}

	void Outlet::addListener( AbstractCallback< TimestampedData > &callback )
	{
		m_InletEvent.addListener( callback );
	}

	void Outlet::removeListener( AbstractCallback< TimestampedData > &callback )
	{
		m_InletEvent.removeListener( callback );
	}
}