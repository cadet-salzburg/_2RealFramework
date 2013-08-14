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
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"
#include "engine/_2RealTimestampedData.h"
#include "datatypes/_2RealCustomData.h"
#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class AbstractInletIO;
	class BasicInlet;

	class BasicInletBuffer : private NonCopyable< BasicInletBuffer >
	{
	public:

		BasicInletBuffer( AbstractInletIO *owner );

		void setData( TimestampedData const& );
		TimestampedData const& getData() const;
		void clearData();

	private:

		AbstractInletIO							*const mOwner;
		mutable Poco::FastMutex					mAccess;
		TimestampedData							mData;

	};

	class AbstractInlet : private NonCopyable< AbstractInlet >
	{

	public:

		AbstractInlet();
		virtual ~AbstractInlet() {}

		virtual std::shared_ptr< BasicInlet >	operator[]( const unsigned int ) = 0;
		virtual bool							canExpand() const = 0;
		virtual unsigned int					getSubInletCount() const = 0;

	};

	class BasicInlet : public AbstractInlet
	{

	public:

		BasicInlet( AbstractInletIO *owner );

		std::shared_ptr< BasicInlet >			operator[]( const unsigned int );
		bool									canExpand() const;
		unsigned int							getSubInletCount() const;

		void									setSelfRef( std::shared_ptr< BasicInlet > );
		std::shared_ptr< const CustomType >		getDataThreadsafe() const;
		std::shared_ptr< const CustomType >		getData() const;
		bool									hasChanged() const;
		void									update( std::shared_ptr< const CustomType > );

	private:

		AbstractInletIO							*const mOwner;
		std::shared_ptr< const CustomType >		mLastData;
		std::shared_ptr< const CustomType >		mData;
		mutable Poco::FastMutex					mAccess;
		std::weak_ptr< BasicInlet >				mSelfRef;

	};

	class MultiInlet : public AbstractInlet
	{

	public:

		MultiInlet( AbstractInletIO *owner );

		std::shared_ptr< BasicInlet >			operator[]( const unsigned int );
		bool									canExpand() const;
		unsigned int							getSubInletCount() const;

	private:

		AbstractInletIO							*const mOwner;

	};
}