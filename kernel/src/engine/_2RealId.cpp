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

	MetainfoId::MetainfoId( std::shared_ptr< const MetainfoId > owner, const MetainfoType type, const std::string name ) :
		mOwner( owner ), mType( type ), mName( name )
	{
	}

	std::string MetainfoId::toString() const
	{
		std::string owner = "";
		auto o = mOwner.lock();
		if ( o != nullptr )
			owner = o->toString() + "\\\\";

		return owner + typeToString( mType ) + mName;
	}

	std::string MetainfoId::getName() const
	{
		return mName;
	}

	bool MetainfoId::operator<( MetainfoId const& other ) const
	{
		return toString() < other.toString();
	}

	//InstanceId::InstanceId( const InstanceType type, const std::string name ) :
	//	mOwner(), mType( type ), mName( name )
	//{
	//}

	//InstanceId::InstanceId( std::shared_ptr< const InstanceId > owner, const InstanceType type, const std::string name ) :
	//	mOwner( owner ), mType( type ), mName( name )
	//{
	//}

	//InstanceId InstanceId::makeId( std::shared_ptr< const InstanceId > owner, const InstanceType type, const std::string name )
	//{
	//	return InstanceId( owner, type, name );
	//}
}
