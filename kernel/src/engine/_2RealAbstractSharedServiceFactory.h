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
#include "engine/_2RealSharedService.h"

#include "engine/_2RealSharedService.h"

namespace _2Real
{
	namespace bundle
	{
		class BlockIo;
	}

	class AbstractSharedServiceFactory
	{
	public:
		virtual ~AbstractSharedServiceFactory() {};
		virtual std::shared_ptr< AbstractSharedService > create( _2Real::bundle::BlockIo const&, std::vector< std::shared_ptr< _2Real::bundle::AbstractBlock > > const& ) const = 0;
		// needed for copying the data
		virtual std::shared_ptr< AbstractSharedServiceFactory > clone() const = 0;
	};
}