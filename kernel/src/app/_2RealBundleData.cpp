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

#include "app/_2RealBundleData.h"

using std::string;
using std::ostream;

namespace _2Real
{
	namespace app
	{
		BundleInfo::BundleInfo()
		{
			m_BundleData.name = "undefined";
			m_BundleData.directory = "undefined";
			m_BundleData.description = "undefined";
			m_BundleData.author = "undefined";
			m_BundleData.contact = "undefined";
			m_BundleData.category = "undefined";
			m_BundleData.version = Version( 0, 0, 0 );
		}

		BundleInfo::BundleInfo( BundleData const& data, BlockInfos const& blocks ) :
			m_BundleData( data ),
			m_ExportedBlocks( blocks )
		{
		}

		string const& BundleInfo::getName() const
		{
			return m_BundleData.name;
		}

		string const& BundleInfo::getInstallDirectory() const
		{
			return m_BundleData.directory;
		}

		string const& BundleInfo::getDescription() const
		{
			return m_BundleData.description;
		}

		string const& BundleInfo::getAuthor() const
		{
			return m_BundleData.author;
		}

		string const& BundleInfo::getContact() const
		{
			return m_BundleData.contact;
		}

		string const& BundleInfo::getCategory() const
		{
			return m_BundleData.category;
		}

		Version const& BundleInfo::getVersion() const
		{
			return m_BundleData.version;
		}

		BundleInfo::BlockInfos const& BundleInfo::getExportedBlocks() const
		{
			return m_ExportedBlocks;
		}
	}
}