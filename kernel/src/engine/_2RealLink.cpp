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

#include "engine/_2RealLink.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"

#include <assert.h>
#include <iostream>

namespace _2Real
{

	AbstractLink::AbstractLink( Parameter &p1, Parameter &p2 ) :
		m_Link1( p1 ),
		m_Link2( p2 )
	{
#ifdef _DEBUG
		if ( &p1 == &p2 )
		{
			assert( NULL );
		}
#endif
	}

	bool AbstractLink::operator<( AbstractLink const& other )
	{
		return ( &m_Link1 < &other.m_Link1 && &m_Link2 < &other.m_Link2 );
	}

	IOLink::IOLink( Inlet &inlet, Outlet &outlet ) :
		AbstractLink( inlet, outlet ),
		m_Inlet( inlet ),
		m_Outlet( outlet )
	{
	}

	void IOLink::activate() 
	{
		m_Outlet.addInletListener( m_Inlet.m_Buffer );
	}

	void IOLink::deactivate()
	{
		m_Outlet.removeInletListener( m_Inlet.m_Buffer );
	}

}