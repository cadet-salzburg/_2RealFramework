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

#include "helpers/_2RealStdIncludes.h"
#include <ostream>

namespace _2Real
{

	enum class MetainfoType { BUNDLE, BLOCK, IOSLOT, BASICTYPE, CUSTOMTYPE, POLICY, UNKNOWN };

	std::string typeToString( const MetainfoType );

	class MetainfoId
	{

	public:

		static std::shared_ptr< const MetainfoId > create( std::shared_ptr< const MetainfoId >, const MetainfoType, const std::string );

		std::string toString() const;
		std::string getName() const;

		bool operator<( MetainfoId const& other ) const;
		bool operator==( MetainfoId const& other ) const;

	private:

		static uint64_t	sCounter;

		MetainfoId( std::shared_ptr< const MetainfoId >, const MetainfoType, const std::string, const uint64_t );

		std::weak_ptr< const MetainfoId >		mParent;
		MetainfoType							mType;
		std::string								mName;
		uint64_t								mId;

	};

	enum class InstanceType { BUNDLE, BLOCK, IOSLOT, TYPE, POLICY, UNKNOWN };

	std::string typeToString( const InstanceType );

	class InstanceId
	{

	public:

		static std::shared_ptr< const InstanceId > create( std::shared_ptr< const MetainfoId >, std::shared_ptr< const InstanceId >, const InstanceType, const std::string );

		std::string toString() const;
		std::string getName() const;

		bool operator<( InstanceId const& other ) const;
		bool operator==( InstanceId const& other ) const;

	private:

		static uint64_t	sCounter;

		InstanceId( std::shared_ptr< const MetainfoId >, std::shared_ptr< const InstanceId >, const InstanceType, const std::string, const uint64_t );

		std::weak_ptr< const MetainfoId >		mMetainfo;
		std::weak_ptr< const InstanceId >		mParent;
		InstanceType							mType;
		std::string								mName;
		uint64_t								mId;

	};

	//class Id
	//{

	//public:

	//	virtual ~Id() = default;

	//	virtual std::string toString() const = 0;
	//	virtual std::string getName() const = 0;

	//	virtual bool operator<( Id const& other ) const = 0;
	//	virtual bool operator==( Id const& other ) const = 0;
	//
	//	virtual std::ostream & print( std::ostream & ) const = 0;
	//	friend std::ostream& operator<<( std::ostream &, Id const& );

	//private:

	//	static uint64_t	sCounter;

	//	explicit Id( const uint64_t );

	//	uint64_t	mId;

	//};

	//class BundleId : public Id
	//{

	//public:

	//	static std::shared_ptr< Id > create( Path const& );

	//	std::ostream & print( std::ostream & ) const;

	//private:

	//	std::string		mName;

	//};

	//class BlockId : public Id
	//{

	//public:

	//	static std::shared_ptr< InstanceId > create( std::shared_ptr< BundleId >, const std::string, const uint64_t );

	//private:

	//	std::string					mName;
	//	uint64_t					mCreationCount;
	//	std::weak_ptr< BundleId >	mParent;

	//};

	//class IoSlotId : public Id
	//{

	//public:

	//	static std::shared_ptr< InstanceId > create( std::shared_ptr< BundleId >, const std::string, const uint64_t );

	//private:

	//	std::string					mName;
	//	std::weak_ptr< BlockId >	mParent;

	//};

}