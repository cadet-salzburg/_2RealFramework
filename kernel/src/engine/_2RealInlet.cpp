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

	Inlet::Inlet( AbstractUberBlock &owningBlock, string const& name, string const& longTypename, string const& type ) :
		HandleAble< bundle::InletHandle >( *this ),
		Parameter( owningBlock, name, longTypename, type )
	{
	}

//	void Inlet::linkTo( app::OutletHandle &outletHandle )
//	{
//		Parameter *p = outletHandle.ParameterHandle::m_Parameter;
//#ifdef _DEBUG
//		if ( p == nullptr ) assert( NULL );
//#endif
//
//		Outlet *outlet = static_cast< Outlet * >( p );
//		Parameter::getOwningUberBlock().createLink( *this, *outlet );
//	}
//
//	void Inlet::unlinkFrom( app::OutletHandle &outletHandle )
//	{
//		Parameter *p = outletHandle.ParameterHandle::m_Parameter;
//#ifdef _DEBUG
//		if ( p == nullptr ) assert( NULL );
//#endif
//
//		Outlet *outlet = static_cast< Outlet * >( p );
//		Parameter::getOwningUberBlock().destroyLink( *this, *outlet );
//	}

}