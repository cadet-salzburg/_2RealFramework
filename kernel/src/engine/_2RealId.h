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
#include "engine/_2RealCloneableMetainfo.h"

namespace _2Real
{

	enum class MetainfoType { BUNDLE, BLOCK, IOSLOT, TYPE, UNKNOWN };

	std::string typeToString( const MetainfoType );

	class MetainfoId
	{

	public:

		MetainfoId( std::shared_ptr< const MetainfoId >, const MetainfoType, const std::string );
		std::string toString() const;
		std::string getName() const;

		bool operator<( MetainfoId const& other ) const;

	private:

		std::weak_ptr< const MetainfoId >		mOwner;
		MetainfoType							mType;
		std::string								mName;

	};

}