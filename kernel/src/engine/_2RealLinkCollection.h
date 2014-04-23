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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealSignals.h"

namespace _2Real
{
	class Link;
	class InletImpl;
	class OutletImpl;

	class LinkCollection
	{
	
	public:

		LinkCollection() = default;
		~LinkCollection() = default;

		LinkCollection( LinkCollection const& other ) = delete;
		LinkCollection( LinkCollection && other ) = delete;
		LinkCollection& operator=( LinkCollection && other ) = delete;
		LinkCollection& operator=( LinkCollection const& other ) = delete;

		void					clear();
		std::shared_ptr< Link > createLink( std::shared_ptr< InletImpl >, std::shared_ptr< OutletImpl > );
		void					linkDestroyed( std::shared_ptr< const Link > );

	private:

		typedef std::vector< std::pair< boost::signals2::connection, std::shared_ptr< Link > > > Links;

		Links mLinks;

	};
}