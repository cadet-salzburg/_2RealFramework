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

namespace _2Real
{
	class Bundle;

	namespace app
	{
		//class BundleMetainfo;
		//class BlockHandle;
		//class ContextBlockHandle;

		class BundleHandle
		{
		public:

			// creates invalid bundle handle, fwiw
			BundleHandle();
			explicit BundleHandle( std::shared_ptr< Bundle > );
			// use to check if underlying bundle is still valid ( =loaded ) if you're unsure
			bool					isValid() const;
				//BundleMetainfo			getBundleMetainfo() const;
			// explicit creation of context block - otherwise, will be created when first block that needs it is created
				//ContextBlockHandle		createContextBlockInstance() const;
				//BlockHandle				createFunctionBlockInstance( std::string const& blockName );
			// unloading currently block s until all block / context block instances are killed
			// no timeout could cause the app to block forever
			void					unload( const long timeout = 1000 );

		private:

			std::weak_ptr< Bundle >		mImpl;

		};

	}
}
