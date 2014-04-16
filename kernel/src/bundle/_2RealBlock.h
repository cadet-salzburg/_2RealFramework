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
#include "bundle/_2RealBlockIo.h"
#include "engine/_2RealBlockAccess.h"

namespace _2Real
{
	namespace bundle
	{

		class Block
		{

		private:

			/*
			*	interface -> needs to be implemented by any dll exported block
			*/

			// must be called at least once before update can be called, may be called more than once
			// parameters can be read during setup
			virtual void setup() = 0;
			// inlets can be read & outlets can be written during update
			virtual void update() = 0;
			// shutdown, called before block is destroied
			virtual void shutdown() = 0;

		public:

			Block( const BlockIo, const std::vector< std::shared_ptr< Block > > );

			Block( Block const& other ) = delete;
			Block( Block && other ) = delete;
			Block& operator=( Block const& other ) = delete;
			Block& operator=( Block && other ) = delete;
			Block() = delete;
			virtual ~Block() = default;

			void doSetup( BlockAccess );
			void doUpdate( BlockAccess );
			void doShutdown( BlockAccess );

		protected:

			BlockIo												mIo;
			std::vector< std::shared_ptr< Block > >				mDependencies;

		};

	}
}