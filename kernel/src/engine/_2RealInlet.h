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

#include "bundle/_2RealInletHandle.h"
#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"
#include "engine/_2RealTimestampedData.h"

#include "datatypes/_2RealCustomData.h"
#include "helpers/_2RealPoco.h"

namespace _2Real
{
	class AbstractInletIO;
	class BasicInlet;

	// same interface as parameterbuffer!!!!!!

	class BasicInletBuffer : private NonCopyable< BasicInletBuffer >
	{
	public:

		BasicInletBuffer( AbstractInletIO *owner );

		void setData( std::shared_ptr< const CustomType > );
		std::shared_ptr< const CustomType > getData() const;

	private:

		AbstractInletIO							*const mOwner;
		mutable Poco::FastMutex					mAccess;
		std::shared_ptr< const CustomType >		mData;

	};

	class AbstractInlet : private NonCopyable< AbstractInlet >, private Handleable< AbstractInlet, bundle::InletHandle >
	{

	public:

		AbstractInlet();
		virtual ~AbstractInlet() {}

		using Handleable< AbstractInlet, bundle::InletHandle >::getHandle;
		using Handleable< AbstractInlet, bundle::InletHandle >::registerHandle;
		using Handleable< AbstractInlet, bundle::InletHandle >::unregisterHandle;

		virtual BasicInlet &					operator[]( const unsigned int index ) = 0;
		virtual bool							isMultiInlet() const = 0;
		virtual unsigned int					getSize() const = 0;

	};

	class BasicInlet : public AbstractInlet
	{

	public:

		BasicInlet( AbstractInletIO *owner );

		BasicInlet &							operator[]( const unsigned int index );
		bool									isMultiInlet() const;
		unsigned int							getSize() const;

		std::shared_ptr< const CustomType >		getDataThreadsafe() const;
		std::shared_ptr< const CustomType >		getData() const;
		bool									hasChanged() const;
		void									update( std::shared_ptr< const CustomType > );

	private:

		AbstractInletIO							*const mOwner;
		std::shared_ptr< const CustomType >		mLastData;
		std::shared_ptr< const CustomType >		mData;
		mutable Poco::FastMutex					mAccess;

	};

	class MultiInlet : public AbstractInlet
	{

	public:

		MultiInlet( AbstractInletIO *owner );

		BasicInlet &							operator[]( const unsigned int index );
		bool									isMultiInlet() const;
		unsigned int							getSize() const;

	private:

		AbstractInletIO							*const mOwner;

	};
}