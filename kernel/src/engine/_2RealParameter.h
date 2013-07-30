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

#include "bundle/_2RealParameterHandle.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealPoco.h"

namespace _2Real
{

	class ParameterIO;

	class ParameterBuffer
	{
	public:

		ParameterBuffer( ParameterIO *owner );

		// called by papplication - anytime
		void setData( std::shared_ptr< const CustomType > );
		// not sure that this is ever needed
		std::shared_ptr< const CustomType > getData() const;

	private:

		ParameterIO								*const mOwner;
		mutable Poco::FastMutex					mAccess;
		std::shared_ptr< const CustomType >		mData;
	};

	class Parameter : private NonCopyable< Parameter >
	{
		
	public:

		Parameter( ParameterIO *owner );

		std::shared_ptr< const CustomType >		getDataThreadsafe() const;
		// called by bundle during update or setup
		std::shared_ptr< const CustomType >		getData() const;
		// called by bundle during setup or update
		bool									hasChanged() const;
		// called by parameter io  before an update
		void									update( std::shared_ptr< const CustomType > );

	private:

		ParameterIO								*const mOwner;
		std::shared_ptr< const CustomType >		mData;
		std::shared_ptr< const CustomType >		mLastData;
		mutable Poco::FastMutex					mAccess;

	};

}