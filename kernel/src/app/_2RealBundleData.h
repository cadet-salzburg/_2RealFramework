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

#include "app/_2RealBlockData.h"
#include "helpers/_2RealVersion.h"

#include <vector>
#include <string>

namespace _2Real
{
	//namespace engine
	//{
		class Bundle;
	//}

	namespace app
	{
		class BundleInfo
		{

			friend class _2Real::Bundle;

		public:

			typedef std::vector< BlockInfo >					BlockInfos;
			typedef std::vector< BlockInfo >::iterator			BlockInfoIterator;
			typedef std::vector< BlockInfo >::const_iterator	BlockInfoConstIterator;

			BundleInfo();
			//BundleInfo( BundleData const& data, BlockInfos const& infos );

			BlockInfos const&			getExportedBlocks() const;
			std::string const&			getInstallDirectory() const;
			std::string const&			getDescription() const;
			std::string const&			getAuthor() const;
			std::string const&			getContact() const;
			std::string const&			getName() const;
			std::string const&			getCategory() const;
			Version const&				getVersion() const;

		private:

			std::string					m_Name;
			std::string					m_InstallDirectory;
			std::string					m_Description;
			std::string					m_Author;
			std::string					m_Contact;
			std::string					m_Category;
			Version						m_Version;
			BlockInfos					m_ExportedBlocks;

		};
	}
}