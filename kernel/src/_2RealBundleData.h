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

#include "_2RealVersion.h"
#include "_2RealBlockData.h"

#include <map>
#include <list>
#include <string>

namespace _2Real
{

	typedef std::map< std::string, BlockData >	BlockDataMap;

	class BundleData
	{

	public:

		BundleData();
		BundleData( BundleData const& src );
		BundleData& operator=( BundleData const& src );
		~BundleData();

		friend std::ostream& operator<<( std::ostream &out, BundleData const& data );

		void													addBlockData( BlockData const& data );
		BlockData	const&										getBlockData( std::string const& blockName ) const;
		BlockDataMap	const&									getExportedBlocks() const;

		void						setDescription( std::string const& description );
		void						setAuthor( std::string const& author );
		void						setContact( std::string const& contact );
		void						setName( std::string const& name );
		void						setInstallDirectory( std::string const& path );
		void						setCategory( std::string const& category );
		void						setVersion( Version const& version );

		std::string const&			getInstallDirectory() const;
		std::string const&			getDescription() const;
		std::string const&			getAuthor() const;
		std::string const&			getContact() const;
		std::string const&			getName() const;
		std::string const&			getCategory() const;
		Version const&				getVersion() const;

	private:

		void clear();

		std::string					m_Name;
		std::string					m_InstallDirectory;
		std::string					m_Description;
		std::string					m_Author;
		std::string					m_Contact;
		std::string					m_Category;
		Version						m_Version;
		BlockDataMap				m_ExportedBlocks;

	};

}