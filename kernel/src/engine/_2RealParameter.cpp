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

#include "engine/_2RealParameter.h"
#include "engine/_2RealBlock.h"
#include "engine/_2RealIoSlotMetainfo.h"
#include "engine/_2RealId.h"

namespace _2Real
{

	std::shared_ptr< Parameter > Parameter::createFromMetainfo( std::shared_ptr< Block > parent, std::shared_ptr< const IoSlotMetainfo > meta )
	{
		std::shared_ptr< const InstanceId > parameterId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::IOSLOT, meta->getId()->getName() );
		std::shared_ptr< Parameter > parameter( new Parameter( parent, meta, parameterId ) );

		std::shared_ptr< DataItem > initValue( new DataItem( meta->getInitialValue() ) );
		parameter->InSlot::setTmpValue( initValue );

		return parameter;
	}

	Parameter::Parameter( std::shared_ptr< Block > parent, std::shared_ptr< const IoSlotMetainfo > meta, std::shared_ptr< const InstanceId > id ) :
		InSlot(),
		std::enable_shared_from_this< Parameter >(),
		mParent( parent ),
		mMetainfo( meta ),
		mId( id )
	{
	}

	std::shared_ptr< const InstanceId > Parameter::getId() const
	{
		return mId;
	}

	std::shared_ptr< Block > Parameter::getParent()
	{
		return mParent.lock();
	}

}
