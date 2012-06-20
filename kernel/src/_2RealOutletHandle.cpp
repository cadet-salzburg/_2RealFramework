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

#include "_2RealOutletHandle.h"
#include "_2RealOutlet.h"
#include "_2RealEngineData.h"

namespace _2Real
{

	OutletHandle::OutletHandle() :
		ParameterHandle( "" ),
		m_Outlet( nullptr )
	{
	}

	OutletHandle::OutletHandle( Outlet &outlet ) :
		ParameterHandle( outlet.getName() ),
		m_Outlet( &outlet )
	{
	}

	OutletHandle::OutletHandle( OutletHandle const& src ) :
		ParameterHandle( src ),
		m_Outlet( src.m_Outlet )
	{
	}

	OutletHandle& OutletHandle::operator=( OutletHandle const& src )
	{
		ParameterHandle::operator=( src );
		m_Outlet = src.m_Outlet;

		return *this;
	}

	EngineData & OutletHandle::getOutletData()
	{
		return m_Outlet->getCurrentData();
	}

	//void OutletHandle::discardOutletData()
	//{
	//	m_Output->discardCurrent();
	//}

	//void OutletHandle::setOutletData( EngineData const& val )
	//{
	//	m_Outlet->setCurrentData( val );
	//}

}