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

#include "engine/_2RealId.h"

namespace _2Real
{
	uint64_t MetainfoId::sCounter = 0;

	std::shared_ptr< const MetainfoId > MetainfoId::create( std::shared_ptr< const MetainfoId > parent, const MetainfoType type, const std::string name )
	{
		return std::shared_ptr< const MetainfoId >( new MetainfoId( parent, type, name, ++sCounter ) );
	}

	std::string typeToString( const MetainfoType t )
	{
		switch ( t )
		{
		case MetainfoType::BUNDLE:
			return "bundle";
		case MetainfoType::TYPE:
			return "customtype";
		case MetainfoType::BLOCK:
			return "block";
		case MetainfoType::IOSLOT:
			return "ioslot";
		default:
			return "undefined";
		}
	}

	MetainfoId::MetainfoId( std::shared_ptr< const MetainfoId > parent, const MetainfoType type, const std::string name, const uint64_t id ) :
		mParent( parent ), mType( type ), mName( name ), mId( id )
	{
	}

	std::string MetainfoId::toString() const
	{
		std::string parent = "";
		auto p = mParent.lock();
		if ( p != nullptr )
			parent = p->toString() + "\\\\";

		return parent + typeToString( mType ) + "\\\\" + mName;
	}

	std::string MetainfoId::getName() const
	{
		return mName;
	}

	bool MetainfoId::operator<( MetainfoId const& other ) const
	{
		return mId < other.mId;
	}

	bool MetainfoId::operator==( MetainfoId const& other ) const
	{
		return mId == other.mId;
	}

	uint64_t InstanceId::sCounter = 0;

	std::shared_ptr< const InstanceId > InstanceId::create( std::shared_ptr< const MetainfoId > meta, std::shared_ptr< const InstanceId > parent, const InstanceType type, const std::string name )
	{
		return std::shared_ptr< const InstanceId >( new InstanceId( meta, parent, type, name, ++sCounter ) );
	}

	std::string typeToString( const InstanceType t )
	{
		switch ( t )
		{
		case InstanceType::BUNDLE:
			return "bundle";
		case InstanceType::TYPE:
			return "customtype";
		case InstanceType::BLOCK:
			return "block";
		case InstanceType::IOSLOT:
			return "ioslot";
		default:
			return "undefined";
		}
	}

	InstanceId::InstanceId( std::shared_ptr< const MetainfoId > meta, std::shared_ptr< const InstanceId > parent, const InstanceType type, const std::string name, const uint64_t id ) :
		mMetainfo( meta ), mParent( parent ), mType( type ), mName( name ), mId( id )
	{
	}

	std::string InstanceId::toString() const
	{
		std::string parent = "";
		auto p = mParent.lock();
		if ( p != nullptr )
			parent = p->toString() + "\\\\";

		return parent + typeToString( mType ) + "\\\\" + mName;
	}

	std::string InstanceId::getName() const
	{
		return mName;
	}

	bool InstanceId::operator<( InstanceId const& other ) const
	{
		return mId < other.mId;
	}

	bool InstanceId::operator==( InstanceId const& other ) const
	{
		return mId == other.mId;
	}
}
