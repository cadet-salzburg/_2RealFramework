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

#pragma once

#include "bundle/_2RealOutletHandle.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	// OutletIO: owns both outlet & outlet buffer

	class OutletIO;

	// outlet buffer: holds most recently written data of an outlet

	class OutletBuffer : private NonCopyable< OutletBuffer >
	{

	public:

		OutletBuffer( OutletIO *owner );

		// called by outlet io after update
		void setData( std::shared_ptr< const CustomType > );
		// called by application - anytime
		std::shared_ptr< const CustomType > getData() const;

	private:

		OutletIO								*const mOwner;
		mutable Poco::FastMutex					mAccess;
		std::shared_ptr< const CustomType >		mData;

	};

	// outlet: readable from the bundle side of things
	// after each update, is reset to a clone of the init value

	class Outlet : private NonCopyable< Outlet >
	{

	public:

		Outlet( OutletIO *owner );

		// called by outlet io after update, synced
		std::shared_ptr< const CustomType > update( std::shared_ptr< CustomType > );
		// called by bundle::outlethandle during update, synced
		std::shared_ptr< CustomType >		getWriteableData();
		// called by bundle::outlethandle during update, synced
		void								discard();

	private:

		OutletIO								*const mOwner;
		bool									mWasDiscarded;
		std::shared_ptr< CustomType >			mData;

	};
}