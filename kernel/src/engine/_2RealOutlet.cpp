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

#include "engine/_2RealOutlet.h"
#include "engine/_2RealSharedServiceOutletMetainfo.h"

namespace _2Real
{

	Outlet::Outlet( std::shared_ptr< const SharedServiceOutletMetainfo > meta ) :
		IoSlot(),
		mMetainfo( meta )
	{
	}

	std::string const& Outlet::getName() const
	{
		return mMetainfo->getName();
	}

	std::shared_ptr< const SharedServiceOutletMetainfo > Outlet::getMetainfo() const
	{
		return mMetainfo;
	}

	DataItem & Outlet::getValue()
	{
		return *mValue.get();
	}

	std::shared_ptr< const DataItem > Outlet::getTmpValue() const
	{
		std::lock_guard< std::mutex > lock( mMutex );
		return mTmpValue;
	}

	void Outlet::update()
	{
		std::lock_guard< std::mutex > lock( mMutex );
	}
}
