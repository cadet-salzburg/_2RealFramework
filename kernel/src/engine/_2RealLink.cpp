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
#include "engine/_2RealTypeMetadata.h"
#include "engine/_2RealIOMetadata.h"

using std::ostringstream;

namespace _2Real
{
	std::shared_ptr< IOLink > IOLink::link( std::shared_ptr< BasicInletIO > inlet, std::shared_ptr< OutletIO >outlet )
	{
		std::shared_ptr< const TypeMetadata > in = inlet->getMetadata()->typeMetadata;
		std::shared_ptr< const TypeMetadata > out = outlet->getMetadata()->typeMetadata;

		std::shared_ptr< IOLink > result;
		std::shared_ptr< const TypeConverter > converterInOut;
		std::shared_ptr< const TypeConverter > converterOutIn;

		if ( in->matches( out.get(), TypeMetadata::TypeMatchSetting::PERFECT_TYPE_MATCH, converterInOut, converterOutIn ) )
			result.reset( new IOLink( inlet, outlet, converterOutIn, converterInOut ) );

		return result;
	}

	IOLink::IOLink( std::shared_ptr< BasicInletIO > i, std::shared_ptr< OutletIO > o, std::shared_ptr< const TypeConverter > forward, std::shared_ptr< const TypeConverter > reverse ) :
		mInletIO( i ),
		mOutletIO( o ),
		mForwardConverter( forward ),
		mReverseConverter( reverse )
	{
	}

	//bool IOLink::isValid() const
	//{
	//	return ( ( mInletIO.get() != nullptr ) && ( mOutletIO.get() != nullptr ) );
	//}

	bool IOLink::operator<( IOLink const& other )
	{
		if ( mInletIO < other.mInletIO )		return true;
		if ( other.mInletIO < mInletIO )		return false;
		if ( mOutletIO < other.mOutletIO )		return true;
		return false;
	}

	//bool IOLink::isInletInvolved( BasicInletIO const& i ) const
	//{
	//	if ( m_InletIO == &i )
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	//bool IOLink::isBlockInvolved( AbstractUberBlock const& b ) const
	//{
	//	return ( mInletIO->belongsToBlock( &b ) || mOutletIO->belongsToBlock( &b ) );
	//}

	void IOLink::activate() 
	{
		// directly link inlet & outlet
		if ( mForwardConverter.get() == nullptr )
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< BasicInletIO, TimestampedData const& >( *( mInletIO.get() ), &BasicInletIO::receiveData );
			mOutletIO->mInletEvent->addListener( *cb );
		}
		else
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< IOLink, TimestampedData const& >( *this, &IOLink::receiveData );
			mOutletIO->mInletEvent->addListener( *cb );
		}
	}

	void IOLink::receiveData( TimestampedData const& data ) 
	{
		std::shared_ptr< const CustomType > t = data.value;
		std::shared_ptr< const CustomType > cv = mForwardConverter->convert( t );
		mInletIO->receiveData( TimestampedData( cv, data.timestamp ) );
	}

	void IOLink::deactivate()
	{
		if ( mForwardConverter.get() == nullptr )
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< BasicInletIO, TimestampedData const& >( *( mInletIO.get() ), &BasicInletIO::receiveData );
			mOutletIO->mInletEvent->removeListener( *cb );
		}
		else
		{
			AbstractCallback< TimestampedData const& > *cb = new MemberCallback< IOLink, TimestampedData const& >( *this, &IOLink::receiveData );
			mOutletIO->mInletEvent->removeListener( *cb );
		}
	}

}