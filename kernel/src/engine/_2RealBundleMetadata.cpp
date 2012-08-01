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

#include "engine/_2RealBundleMetadata.h"
#include "engine/_2RealBlockMetadata.h"
#include "helpers/_2RealException.h"

#include <sstream>

using std::ostringstream;

namespace _2Real
{

	BundleMetadata::BundleMetadata() :
		m_Name( "undefined" ),
		m_Description( "undefined" ),
		m_Author( "undefined" ),
		m_Contact( "undefined" ),
		m_InstallDirectory( "undefined" ),
		m_Category( "undefined" ),
		m_Version( 0, 0, 0 )
	{
	}

	void BundleMetadata::setInstallDirectory( std::string const& dir )
	{
		m_InstallDirectory = dir;
	}

	void BundleMetadata::setDescription( std::string const& desc )
	{
		m_Description = desc;
	}

	void BundleMetadata::setVersion( Version const& version )
	{
		m_Version = version;
	}

	void BundleMetadata::setAuthor( std::string const& author )
	{
		m_Author = author;
	}

	void BundleMetadata::setContact( std::string const& contact )
	{
		m_Contact = contact;
	}

	void BundleMetadata::setName( std::string const& name )
	{
		m_Name = name;
	}

	void BundleMetadata::setCategory( std::string const& category )
	{
		m_Category = category;
	}

	std::string const& BundleMetadata::getName() const
	{
		return m_Name;
	}

	std::string const& BundleMetadata::getInstallDirectory() const
	{
		return m_InstallDirectory;
	}

	std::string const& BundleMetadata::getDescription() const
	{
		return m_Description;
	}

	std::string const& BundleMetadata::getAuthor() const
	{
		return m_Author;
	}

	std::string const& BundleMetadata::getContact() const
	{
		return m_Contact;
	}

	std::string const& BundleMetadata::getCategory() const
	{
		return m_Category;
	}

	Version const& BundleMetadata::getVersion() const
	{
		return m_Version;
	}

	BlockMetadata const& BundleMetadata::getBlockData( std::string const& name ) const
	{
		BundleMetadata::BlockMetadataConstIterator it = m_ExportedBlocks.find( name );

		if ( it == m_ExportedBlocks.end() )
		{
			ostringstream msg;
			msg << "block " << name << " not found";
			throw NotFoundException( msg.str() );
		}

		return *it->second;
	}

	void BundleMetadata::addBlockData( BlockMetadata const& data )
	{
		std::string blockName = data.getName();
		m_ExportedBlocks[ blockName ] = &data;
	}

	BundleMetadata::BlockMetadatas const& BundleMetadata::getExportedBlocks() const
	{
		return m_ExportedBlocks;
	}

}