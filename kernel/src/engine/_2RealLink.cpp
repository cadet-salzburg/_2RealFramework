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
#include "helpers/_2RealException.h"
#include "engine/_2RealAbstractIOManager.h"
#include "helpers/_2RealCallback.h"
#include "helpers/_2RealEvent.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealInletBuffer.h"

#include <sstream>

using std::ostringstream;

namespace _2Real
{
	IOLink * IOLink::link( InletIO &inlet, OutletIO &outlet )
	{
		if ( inlet.m_Inlet->getLongTypename() != outlet.m_Outlet->getLongTypename() )
		{
			return nullptr;
		}
		else return new IOLink( inlet, outlet );
	}

	bool IOLink::canAutoConvert( InletIO &inlet, OutletIO &outlet )
	{
		const Type tSrc = outlet.m_Outlet->getType();
		const Type tDst = inlet.m_Inlet->getType();
		if ( !( tSrc == tDst ) && !( ( tSrc == Type::VECTOR ) || ( tDst == Type::VECTOR ) || ( tSrc == Type::LIST ) || ( tDst == Type::LIST ) ) )
		{
			const TypeCategory cSrc = outlet.m_Outlet->getTypeCategory();
			const TypeCategory cDst = inlet.m_Inlet->getTypeCategory();
			if ( cSrc == cDst && cSrc == TypeCategory::ARITHMETHIC )
			{
				return true;
			}
		}
		return false;
	}

	const std::string IOLink::findConversion( InletIO &inlet, OutletIO &outlet )
	{
		std::string inType = inlet.m_Inlet->getTypename();
		std::string outType = outlet.m_Outlet->getTypename();

		for ( unsigned int i = 0; i<inType.length(); ++i )
		{
			if ( inType[i] == ' ' ) inType[i] = '_';
		}
		for ( unsigned int i = 0; i<outType.length(); ++i )
		{
			if ( outType[i] == ' ' ) outType[i] = '_';
		}

		std::string conversion = outType + "_to_" + inType;

		return conversion;
	}

	IOLink::IOLink( InletIO &inlet, OutletIO &outlet ) :
		m_InletIO( inlet ),
		m_OutletIO( outlet )
	{
		// no more typechecking here
	}

	InletIO const& IOLink::getInletIO() const
	{
		return m_InletIO;
	}

	OutletIO const& IOLink::getOutletIO() const
	{
		return m_OutletIO;
	}

	bool IOLink::operator<( IOLink const& other )
	{
		if ( m_InletIO.m_Inlet < other.m_InletIO.m_Inlet ) return true;
		if ( other.m_InletIO.m_Inlet < m_InletIO.m_Inlet ) return false;
		if ( m_OutletIO.m_Outlet < other.m_OutletIO.m_Outlet ) return true;
		return false;
	}

	bool IOLink::isBlockInvolved( AbstractUberBlock const& b ) const
	{
		if ( &m_InletIO.m_Inlet->getOwningUberBlock() == &b )
		{
			return true;
		}
		else if ( &m_OutletIO.m_Outlet->getOwningUberBlock() == &b )
		{
			return true;
		}

		return false;
	}

	void IOLink::activate() 
	{
		AbstractCallback< TimestampedData const& > *cb = new MemberCallback< InletBuffer, TimestampedData const& >( *m_InletIO.m_Buffer, &InletBuffer::receiveData );
		m_OutletIO.m_InletEvent->addListener( *cb );
	}

	void IOLink::deactivate()
	{
		AbstractCallback< TimestampedData const& > *cb = new MemberCallback< InletBuffer, TimestampedData const& >( *m_InletIO.m_Buffer, &InletBuffer::receiveData );
		m_OutletIO.m_InletEvent->removeListener( *cb );
	}

}