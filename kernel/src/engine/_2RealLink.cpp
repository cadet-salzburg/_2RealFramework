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
#include "datatypes/_2RealTypeConverter.h"

#include <sstream>

using std::ostringstream;

namespace _2Real
{
	IOLink * IOLink::link( BasicInletIO &inlet, OutletIO &outlet )
	{
		TypeMetadata const& in = inlet.info().type;
		TypeMetadata const& out = outlet.info().type;

		IOLink *l = nullptr;
		TypeConverter const* converterInOut = nullptr;
		TypeConverter const* converterOutIn = nullptr;
		if ( in.matches( out, TypeMetadata::TypeMatchSetting::PERFECT_MATCH, converterInOut, converterOutIn ) )
			l = new IOLink( inlet, outlet, converterOutIn );

		delete converterInOut;

		return l;
	}

	IOLink::IOLink() : m_InletIO( nullptr ), m_OutletIO( nullptr )
	{
	}

	IOLink::~IOLink()
	{
		delete mConverter;
	}

	IOLink::IOLink( BasicInletIO &inlet, OutletIO &outlet, TypeConverter const* cv ) :
		m_InletIO( &inlet ),
		m_OutletIO( &outlet ),
		mConverter( cv )
	{
	}

	BasicInletIO const& IOLink::getInletIO() const
	{
		return *m_InletIO;
	}

	OutletIO const& IOLink::getOutletIO() const
	{
		return *m_OutletIO;
	}

	BasicInletIO& IOLink::getInletIO()
	{
		return *m_InletIO;
	}

	OutletIO& IOLink::getOutletIO()
	{
		return *m_OutletIO;
	}

	bool IOLink::isValid() const
	{
		return ( m_InletIO && m_OutletIO );
	}

	bool IOLink::operator<( IOLink const& other )
	{
		if ( m_InletIO->getInletPtr() < other.m_InletIO->getInletPtr() ) return true;
		if ( other.m_InletIO->getInletPtr() < m_InletIO->getInletPtr() ) return false;
		if ( m_OutletIO->m_Outlet < other.m_OutletIO->m_Outlet ) return true;
		return false;
	}

	bool IOLink::isInletInvolved( BasicInletIO const& i ) const
	{
		if ( m_InletIO == &i )
		{
			return true;
		}
		return false;
	}

	bool IOLink::isBlockInvolved( AbstractUberBlock const& b ) const
	{
		if ( m_InletIO->belongsToBlock( &b ) )
		{
			return true;
		}
		else if ( &m_OutletIO->m_Outlet->getOwningUberBlock() == &b )
		{
			return true;
		}

		return false;
	}

	void IOLink::activate() 
	{
		// directly link inlet & outlet
		if ( mConverter == nullptr )
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< BasicInletBuffer, TimestampedData const& >( m_InletIO->getBuffer(), &BasicInletBuffer::receiveData );
			m_OutletIO->m_InletEvent->addListener( *cb );
		}
		else
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< IOLink, TimestampedData const& >( *this, &IOLink::receiveData );
			m_OutletIO->m_InletEvent->addListener( *cb );
		}
	}

	void IOLink::receiveData( TimestampedData const& data ) 
	{
#ifdef _DEBUG
		assert( mConverter );
#endif
		std::shared_ptr< const CustomType > t = data.value;
		std::shared_ptr< const CustomType > cv = mConverter->convert( t );
		m_InletIO->getBuffer().receiveData( TimestampedData( cv, data.timestamp ) );
	}

	void IOLink::deactivate()
	{
		if ( mConverter == nullptr )
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< BasicInletBuffer, TimestampedData const& >( m_InletIO->getBuffer(), &BasicInletBuffer::receiveData );
			m_OutletIO->m_InletEvent->removeListener( *cb );
		}
		else
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< IOLink, TimestampedData const& >( *this, &IOLink::receiveData );
			m_OutletIO->m_InletEvent->removeListener( *cb );
		}
	}

}