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

#include "app/_2RealCommon.h"
#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealVersion.h"

namespace _2Real
{
	class BundleMetadata;

	namespace app
	{
		class BlockMetainfo;

		class BundleMetainfo
		{

		public:

			BundleMetainfo();
			explicit BundleMetainfo( std::shared_ptr< const BundleMetadata > );
			bool isValid() const;
			std::string const&		getName() const;
			std::string const&		getInstallDirectory() const;
			std::string const&		getDescription() const;
			std::string const&		getAuthor() const;
			std::string const&		getContact() const;
			std::string const&		getCategory() const;
			Version const&			getVersion() const;
			bool					exportsContext() const;
			void getExportedBlocks( std::vector< BlockMetainfo > & ) const;

		private:

			std::weak_ptr< const BundleMetadata >		mImpl;

		};
	}
}