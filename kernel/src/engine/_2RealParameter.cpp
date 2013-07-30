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

namespace _2Real
{
	ParameterBuffer::ParameterBuffer( ParameterIO *owner ) :
		mOwner( owner )
	{
	}

	void ParameterBuffer::setData( std::shared_ptr< const CustomType > newData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		mData = newData;
	}

	std::shared_ptr< const CustomType > ParameterBuffer::getData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		return mData;
	}

	Parameter::Parameter( ParameterIO *owner ) :
		NonCopyable< Parameter >(),
		mOwner( owner )
	{
	}

	std::shared_ptr< const CustomType > Parameter::getDataThreadsafe() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		return mData;
	}

	std::shared_ptr< const CustomType > Parameter::getData() const
	{
		return mData;
	}

	bool Parameter::hasChanged() const
	{
		if ( ( mData.get() == nullptr ) ^ ( mLastData.get() == nullptr ) ) return true;
		else if ( ( mData.get() == nullptr ) || ( mLastData.get() == nullptr ) ) return false;
		else return !( mData->isEqualTo( *( mLastData.get() ) ) );
	}

	void Parameter::update( std::shared_ptr< const CustomType > newData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		mLastData = mData;
		mData = newData;
	}
}