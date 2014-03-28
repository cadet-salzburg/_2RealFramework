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
	MetainfoId::MetainfoId( const MetainfoType type, const std::string name ) :
		mOwner(), mType( type ), mName( name )
	{
	}

	MetainfoId::MetainfoId( std::shared_ptr< const MetainfoId > owner, const MetainfoType type, const std::string name ) :
		mOwner( owner ), mType( type ), mName( name )
	{
	}

	MetainfoId MetainfoId::makeId( std::shared_ptr< const MetainfoId > owner, const MetainfoType type, const std::string name )
	{
		return MetainfoId( owner, type, name );
	}

	InstanceId::InstanceId( const InstanceType type, const std::string name ) :
		mOwner(), mType( type ), mName( name )
	{
	}

	InstanceId::InstanceId( std::shared_ptr< const InstanceId > owner, const InstanceType type, const std::string name ) :
		mOwner( owner ), mType( type ), mName( name )
	{
	}

	InstanceId InstanceId::makeId( std::shared_ptr< const InstanceId > owner, const InstanceType type, const std::string name )
	{
		return InstanceId( owner, type, name );
	}
}
