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
		class BundleInternal;
	//}

	namespace app
	{
		class BundleData
		{

			friend class _2Real::BundleInternal;

		public:

			// (?) are we going to keep this as a vector for sure, or
			// would it be wiser to apply a form of type erasure?
			// at this point, i don't think its worth the extra effort
			typedef std::vector< BlockData >					Blocks;
			typedef std::vector< BlockData >::iterator			BlockIterator;
			typedef std::vector< BlockData >::const_iterator	BlockConstIterator;

			BundleData();

			Blocks const&				getExportedBlocks() const;
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
			Blocks						m_ExportedBlocks;

		};
	}
}