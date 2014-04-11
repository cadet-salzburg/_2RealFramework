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

#include "engine/_2RealInSlot.h"
#include "engine/_2RealCustomdata.h"

namespace _2Real
{
	InSlot::InSlot()
	{
	}

	InSlot::~InSlot()
	{
	}

	void InSlot::setTmpValue( std::shared_ptr< const DataItem > value )
	{
		std::lock_guard< std::mutex > lock( mMutex );
		mTmpValue = value;
	}

	void InSlot::setTmpValueExt( DataItem value )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		// can't move the value directly into mTmpValue, as this would overwrite
		// a value that might be shared by several inlets
		// -> create a new shared ptr and move the data in there instead!
		std::shared_ptr< DataItem > tmp( new DataItem );
		*tmp.get() = std::move( value );
		mTmpValue = tmp;
	}

	DataItem const& InSlot::getValue() const
	{
#ifdef _DEBUG
		assert( mValue );
#endif
		return *mValue.get();
	}

	//std::shared_ptr< DataItem > InSlot::getTmpValue() const
	//{
	//	std::lock_guard< std::mutex > lock( mMutex );
	//	return mTmpValue;
	//}

	void InSlot::update()
	{
		std::lock_guard< std::mutex > lock( mMutex );
		mValue = mTmpValue;
	}
}
