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

#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealAbstractUberBlock.h"

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "inlet handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		InletHandle::InletHandle() :
			ParamHandle(),
			m_Inlet( nullptr )
		{
		}

		InletHandle::InletHandle( Inlet &inlet ) :
			ParamHandle( inlet ),
			m_Inlet( &inlet )
		{
		}

		AppData InletHandle::getCurrentInput() const
		{
			checkHandle( m_Inlet );
			EngineData data = m_Inlet->getData();
			return AppData( data, m_Inlet->getTypename(), m_Inlet->getName() );
		}

		void InletHandle::linkTo( OutletHandle &outlet )
		{
			checkHandle( m_Inlet );
			m_Inlet->linkTo( outlet );
		}

		void InletHandle::unlinkFrom( OutletHandle &outlet )
		{
			checkHandle( m_Inlet );
			m_Inlet->unlinkFrom( outlet );
		}

		void InletHandle::setUpdatePolicy( const InletHandle::InletUpdatePolicy p )
		{
			checkHandle( m_Inlet );
			if ( p == InletHandle::DATA_NEW )
			{
				m_Inlet->getOwningUberBlock().updateWhenInletDataNew( *m_Inlet );
			}
			else if ( p == InletHandle::DATA_VALID )
			{
				m_Inlet->getOwningUberBlock().updateWhenInletDataValid( *m_Inlet );
			}
		}

		void InletHandle::setDefaultValue( EngineData const& data )
		{
			checkHandle( m_Inlet );
			m_Inlet->setDefaultValue( data );
		}
	}
}