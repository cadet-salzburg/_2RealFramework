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

#include "helpers/_2RealHandle.h"
#ifdef _UNIX
    #include <limits.h> // for the limits defs
#endif

#include <string>

namespace _2Real
{
	class Bundle;

	namespace app
	{
		class BlockHandle;
		class ContextBlockHandle;
		class BundleInfo;

		class BundleHandle : private Handle
		{

		public:

			BundleHandle();
			BundleHandle( Bundle &bundle );
			~BundleHandle();
			BundleHandle( BundleHandle const& other );
			BundleHandle& operator=( BundleHandle const& other );

			BundleInfo const& getBundleInfo() const;

			bool isValid() const;
			bool operator==( BundleHandle const& other ) const;
			bool operator!=( BundleHandle const& other ) const;
			bool operator<( BundleHandle const& other ) const;
			bool operator<=( BundleHandle const& other ) const;
			bool operator>( BundleHandle const& other ) const;
			bool operator>=( BundleHandle const& other ) const;

			ContextBlockHandle & getContextBlock() const;
			BlockHandle & createBlockInstance( std::string const& blockName );

			void unload( const long blockTimeout = LONG_MAX );

		protected:

			void invalidate();

		private:

			Bundle		*m_Bundle;

		};

	}
}
