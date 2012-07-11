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

#include "helpers/_2RealVersion.h"
#include "app/_2RealBlockInfo.h"

#include <vector>
#include <string>

namespace _2Real
{
	namespace app
	{
		class BundleInfo
		{

		public:

			struct BundleData
			{
				std::string					name;
				std::string					directory;
				std::string					description;
				std::string					author;
				std::string					contact;
				std::string					category;
				Version						version;
			};

			typedef std::vector< BlockInfo >					BlockInfos;
			typedef std::vector< BlockInfo >::iterator			BlockInfoIterator;
			typedef std::vector< BlockInfo >::const_iterator	BlockInfoConstIterator;

			BundleInfo();
			BundleInfo( BundleData const& data, BlockInfos const& blocks );

			BlockInfos const&			getExportedBlocks() const;
			std::string const&			getInstallDirectory() const;
			std::string const&			getDescription() const;
			std::string const&			getAuthor() const;
			std::string const&			getContact() const;
			std::string const&			getName() const;
			std::string const&			getCategory() const;
			Version const&				getVersion() const;

		private:

			BundleData					m_BundleData;
			BlockInfos					m_ExportedBlocks;

		};
	}
}