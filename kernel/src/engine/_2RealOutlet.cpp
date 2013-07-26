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
#include "datatypes/_2RealCustomData.h"

namespace _2Real
{
	OutletBuffer::OutletBuffer( OutletIO *owner ) :
		NonCopyable< OutletBuffer >(),
		mOwner( owner )
	{
	}

	std::shared_ptr< const CustomType > OutletBuffer::getData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		return mData;
	}

	void OutletBuffer::setData( std::shared_ptr< const CustomType > newData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		mData = newData;
	}

	Outlet::Outlet( OutletIO *owner ) :
		NonCopyable< Outlet >(),
		Handleable< Outlet, bundle::OutletHandle >( *this ),
		mOwner( owner ),
		mWasDiscarded( false )
	{
	}

	std::shared_ptr< const CustomType > Outlet::update( std::shared_ptr< CustomType > newData )
	{
		std::shared_ptr< const CustomType > oldData = mWasDiscarded ? std::shared_ptr< const CustomType >() : mData;
		mData = newData;
		mWasDiscarded = false;
		return oldData;
	}

	std::shared_ptr< CustomType > Outlet::getWriteableData()
	{
		return mData;
	}

	void Outlet::discard()
	{
		mWasDiscarded = true;
	}

}