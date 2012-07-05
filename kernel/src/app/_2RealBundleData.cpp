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

#include <iostream>

using std::string;
using std::ostream;

namespace _2Real
{
	namespace app
	{
		BundleInfo::BundleInfo() :
			m_Name( "undefined" ),
			m_Description( "undefined" ),
			m_Author( "undefined" ),
			m_Contact( "undefined" ),
			m_InstallDirectory( "undefined" ),
			m_Category( "undefined" ),
			m_Version( 0, 0, 0 )
		{
		}

		string const& BundleInfo::getName() const
		{
			return m_Name;
		}

		string const& BundleInfo::getInstallDirectory() const
		{
			return m_InstallDirectory;
		}

		string const& BundleInfo::getDescription() const
		{
			return m_Description;
		}

		string const& BundleInfo::getAuthor() const
		{
			return m_Author;
		}

		string const& BundleInfo::getContact() const
		{
			return m_Contact;
		}

		string const& BundleInfo::getCategory() const
		{
			return m_Category;
		}

		Version const& BundleInfo::getVersion() const
		{
			return m_Version;
		}

		BundleInfo::BlockInfos const& BundleInfo::getExportedBlocks() const
		{
			return m_ExportedBlocks;
		}
	}
}