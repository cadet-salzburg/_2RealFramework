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
#include "engine/_2RealSharedServiceInletMetainfo.h"
#include "engine/_2RealLink.h"

namespace _2Real
{

	Inlet::Inlet( std::shared_ptr< const SharedServiceInletMetainfo > meta ) :
		InSlot(), AbstractInlet( meta ), DataSink(), std::enable_shared_from_this< Inlet >()
	{
		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		InSlot::setTmpValue( initValue );
	}

	void Inlet::update()
	{
		InSlot::update();
	}

	void Inlet::receiveData( std::shared_ptr< const DataItem > data )
	{
		InSlot::setTmpValue( data );
	}

	std::shared_ptr< Link > Inlet::linkTo( std::shared_ptr< DataSource > source )
	{
		// inlet keeps tracks of it's links
		// ? might be problematic if the load - save thing comes back ?
		mLink.reset( new Link( source, std::static_pointer_cast< DataSink >( shared_from_this() ) ) );
		return mLink;
	}

	std::shared_ptr< const TMetainfo > Inlet::getTypeMetainfo() const
	{
		return mMetainfo->getTypeMetainfo();
	}
}
