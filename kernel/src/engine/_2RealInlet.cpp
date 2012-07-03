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

#include "engine/_2RealInlet.h"
#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealOutlet.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "app/_2RealOutletHandle.h"
#include "engine/_2RealEngineImpl.h"

#ifdef _DEBUG
	#include <assert.h>
#endif

using std::string;

namespace _2Real
{

	Inlet::Inlet( AbstractUberBlock &owner, string const& name, string const& longTypename, string const& type, EngineData const& defaultValue ) :
		Parameter( owner, name, longTypename, type ),
		m_Buffer( defaultValue ),
		m_CurrentData( EngineData(), 0 ),
		m_LastData( EngineData(), 0 )
	{
	}

	void Inlet::updateCurrentValue()
	{
		//m_CurrentData = m_Buffer.getCurrentData();
		Parameter::setData( m_Buffer.getCurrentData() );
		Parameter::synchronize();			// immediately makes changes visible to inlet and app
	}

	void Inlet::updateDataBuffer()
	{
		m_Buffer.updateDataBuffer();
	}

	void Inlet::setDefaultValue( EngineData const& data )
	{
		m_Buffer.setDefaultData( TimestampedData( data, EngineImpl::instance().getElapsedTime() ) );
	}

	void Inlet::disableTriggering( TimestampedData const& data )
	{
		m_Buffer.disableTriggering( data );
	}

	void Inlet::registerUpdateTrigger( AbstractInletBasedTrigger &trigger )
	{
		m_Buffer.registerUpdateTrigger( trigger );
	}

	void Inlet::unregisterUpdateTrigger( AbstractInletBasedTrigger &trigger )
	{
		m_Buffer.unregisterUpdateTrigger( trigger );
	}

	void Inlet::linkTo( app::OutletHandle &outletHandle )
	{
		Param *p = outletHandle.m_Param;
#ifdef _DEBUG
		if ( p == nullptr ) assert( NULL );
#endif

		Outlet *outlet = dynamic_cast< Outlet * >( p );
#ifdef _DEBUG
		if ( outlet == nullptr ) assert( NULL );
#endif

		m_Owner.createLink( *this, *outlet );
	}

	void Inlet::unlinkFrom( app::OutletHandle &outletHandle )
	{
		Param *p = outletHandle.m_Param;
#ifdef _DEBUG
		if ( p == nullptr ) assert( NULL );
#endif

		Outlet *outlet = dynamic_cast< Outlet * >( p );
#ifdef _DEBUG
		if ( outlet == nullptr ) assert( NULL );
#endif

		m_Owner.destroyLink( *this, *outlet );
	}

}