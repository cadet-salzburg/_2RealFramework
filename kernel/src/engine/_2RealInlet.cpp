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
#include "bundle/_2RealInletHandle.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealAbstractIOManager.h"

using std::string;

namespace _2Real
{
	BasicInletBuffer::BasicInletBuffer( AbstractInletIO *owner ) :
		mOwner( owner )
	{
	}

	void BasicInletBuffer::setData( std::shared_ptr< const CustomType > data )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		mData = data;
	}

	std::shared_ptr< const CustomType > BasicInletBuffer::getData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		return mData;
	}

	AbstractInlet::AbstractInlet() :
		NonCopyable< AbstractInlet >(),
		Handleable< AbstractInlet, bundle::InletHandle >( *this )
	{
	}

	BasicInlet::BasicInlet( AbstractInletIO *owner ) :
		mOwner( owner )
	{
	}

	BasicInlet & BasicInlet::operator[]( const unsigned int index )
	{
		return *this;
	}

	bool BasicInlet::isMultiInlet() const { return false; }
	
	unsigned int BasicInlet::getSize() const
	{
		return 1;
	}

	std::shared_ptr< const CustomType > BasicInlet::getData() const
	{
		return mData;
	}

	std::shared_ptr< const CustomType > BasicInlet::getDataThreadsafe() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		return mData;
	}

	bool BasicInlet::hasChanged() const
	{
		if ( ( mData.get() == nullptr ) ^ ( mLastData.get() == nullptr ) ) return true;
		else if ( ( mData.get() == nullptr ) || ( mLastData.get() == nullptr ) ) return false;
		else return !( mData->isEqualTo( *( mLastData.get() ) ) );
	}

	void BasicInlet::update( std::shared_ptr< const CustomType > data )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		mLastData = mData;
		mData = data;
	}

	MultiInlet::MultiInlet( AbstractInletIO *owner ) :
		mOwner( owner )
	{
	}

	BasicInlet & MultiInlet::operator[]( const unsigned int index )
	{
		return mOwner->operator[]( index ).getInlet();
	}

	bool MultiInlet::isMultiInlet() const { return true; }
	
	unsigned int MultiInlet::getSize() const
	{
		return mOwner->getSize();
	}
}