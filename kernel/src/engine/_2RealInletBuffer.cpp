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

#include "engine/_2RealInletBuffer.h"

using std::stringstream;

namespace _2Real
{
	class AbstractInsertionPolicy
	{
	public:
		virtual ~AbstractInsertionPolicy() {};
		virtual bool				insertData( TimestampedData const& data, DataQueue::DataBuffer &buffer ) = 0;
		virtual void				setMaxCapacity( const unsigned int max ) = 0;
		virtual unsigned int		getMaxCapacity() const = 0;
	};

	class RemoveOldest : public AbstractInsertionPolicy
	{
	public:
		RemoveOldest( const unsigned int max );
		bool						insertData( TimestampedData const& data, DataQueue::DataBuffer &buffer );
		void						setMaxCapacity( const unsigned int max );
		unsigned int				getMaxCapacity() const;
	private:
		mutable Poco::FastMutex		mAccess;
		unsigned int				mMaxCapacity;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RemoveOldest::RemoveOldest( const unsigned int max ) :
		mMaxCapacity( max )
	{
	}

	bool RemoveOldest::insertData( TimestampedData const& data, DataQueue::DataBuffer &buffer )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		while ( buffer.size() >= mMaxCapacity && !buffer.empty() )
		{
			// TODO: some sort of overflow cb for the app i guess
			buffer.pop_front();
		}

		buffer.push_back( data );
		return true;
	}

	void RemoveOldest::setMaxCapacity( const unsigned int max )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		mMaxCapacity = max;
	}

	unsigned int RemoveOldest::getMaxCapacity() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		return mMaxCapacity;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DataQueue::DataQueue( AbstractInletIO *owner, const unsigned int capacity ) :
		mOwner( owner ),
		mInsertionPolicy( new RemoveOldest( capacity ) )
	{
	}

	DataQueue::~DataQueue()
	{
		delete mInsertionPolicy;
	}

	void DataQueue::storeDataItem( TimestampedData const& data )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		mInsertionPolicy->insertData( data, mBuffer );
	}

	bool DataQueue::getDataItem( TimestampedData &data )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );
		if ( mBuffer.empty() )
			return false;
		else
		{
			data = mBuffer.front();
			mBuffer.pop_front();
			return true;
		}
	}

	void DataQueue::setMaxCapacity( const unsigned int size )
	{
		mInsertionPolicy->setMaxCapacity( size );
	}

	unsigned int DataQueue::getMaxCapacity() const
	{
		return mInsertionPolicy->getMaxCapacity();
	}

}