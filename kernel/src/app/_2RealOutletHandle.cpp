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

#include "app/_2RealOutletHandle.h"
#include "app/_2RealInletHandle.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealAbstractUberBlock.h"

#define checkHandle( obj )\
	if ( obj == nullptr ) throw UninitializedHandleException( "block handle not initialized" );\

namespace _2Real
{
	namespace app
	{
		OutletHandle::OutletHandle() :
			ParamHandle(),
			m_Outlet( nullptr )
		{
		}

		OutletHandle::OutletHandle( Outlet &outlet ) :
			ParamHandle( outlet ),
			m_Outlet( &outlet )
		{
		}

		AppData OutletHandle::getLastOutput() const
		{
			EngineData data = m_Outlet->getData();
			return AppData( data, m_Outlet->getTypename(), m_Outlet->getName() );
		}

		void OutletHandle::linkTo( InletHandle &inlet )
		{
			m_Outlet->linkTo( inlet );
		}

		void OutletHandle::unlinkFrom( InletHandle &inlet )
		{
			m_Outlet->unlinkFrom( inlet );
		}

		void OutletHandle::registerToNewData( OutletDataCallback callback, void *userData )
		{
			m_Outlet->registerToNewData( callback, userData );
		}

		void OutletHandle::unregisterFromNewData( OutletDataCallback callback, void *userData )
		{
			m_Outlet->unregisterFromNewData( callback, userData );
		}

		void OutletHandle::registerToNewDataInternal( AbstractOutletDataCallbackHandler &handler )
		{
			m_Outlet->registerToNewData( handler );
		}

		void OutletHandle::unregisterFromNewDataInternal( AbstractOutletDataCallbackHandler &handler )
		{
			m_Outlet->unregisterFromNewData( handler );
		}
	}
}