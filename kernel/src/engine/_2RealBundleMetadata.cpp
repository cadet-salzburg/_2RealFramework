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
#include "engine/_2RealExportMetainfo.h"
#include "helpers/_2RealException.h"

namespace _2Real
{

	BundleMetadata::BundleMetadata( Metainfo *info ) :
		mOwner( info ),
		mName( "undefined" ),
		mDescription( "undefined" ),
		mAuthor( "undefined" ),
		mContact( "undefined" ),
		mInstallDirectory( "undefined" ),
		mCategory( "undefined" ),
		mVersion( 0, 0, 0 )
	{
	}

	void BundleMetadata::setInstallDirectory( std::string const& dir )
	{
		mInstallDirectory = dir;
	}

	void BundleMetadata::setDescription( std::string const& desc )
	{
		mDescription = desc;
	}

	void BundleMetadata::setVersion( Version const& version )
	{
		mVersion = version;
	}

	void BundleMetadata::setAuthor( std::string const& author )
	{
		mAuthor = author;
	}

	void BundleMetadata::setContact( std::string const& contact )
	{
		mContact = contact;
	}

	void BundleMetadata::setName( std::string const& name )
	{
		mName = name;
	}

	void BundleMetadata::setCategory( std::string const& category )
	{
		mCategory = category;
	}

	std::string const& BundleMetadata::getName() const
	{
		return mName;
	}

	std::string const& BundleMetadata::getInstallDirectory() const
	{
		return mInstallDirectory;
	}

	std::string const& BundleMetadata::getDescription() const
	{
		return mDescription;
	}

	std::string const& BundleMetadata::getAuthor() const
	{
		return mAuthor;
	}

	std::string const& BundleMetadata::getContact() const
	{
		return mContact;
	}

	std::string const& BundleMetadata::getCategory() const
	{
		return mCategory;
	}

	Version const& BundleMetadata::getVersion() const
	{
		return mVersion;
	}

	std::shared_ptr< BlockMetadata > BundleMetadata::exportFunctionBlock( bundle::AbstractBlockCreator *ctor, std::string const& name )
	{
		return mOwner->exportFunctionBlock( ctor, name );
	}

	std::shared_ptr< BlockMetadata > BundleMetadata::exportContextBlock( bundle::AbstractBlockCreator *ctor )
	{
		return mOwner->exportContextBlock( ctor );
	}

	std::shared_ptr< TypeMetadata > BundleMetadata::exportCustomType( std::string const& name )
	{
		return mOwner->exportCustomType( name );
	}

	std::shared_ptr< const BlockMetadata > BundleMetadata::getContextBlockMetadata() const
	{
		return mOwner->getContextBlockMetadata();
	}

	std::shared_ptr< const BlockMetadata > BundleMetadata::getFunctionBlockMetadata( std::string const& name ) const
	{
		return mOwner->getFunctionBlockMetadata( name );
	}

	void BundleMetadata::getBlockMetadata( std::vector< std::shared_ptr< const BlockMetadata > > &blocks ) const
	{
		return mOwner->getExportedBlocks( blocks );
	}

	bool BundleMetadata::exportsContext() const
	{
		return mOwner->exportsContext();
	}

	bool BundleMetadata::hasContext() const
	{
		return mOwner->hasContext();
	}

	unsigned int BundleMetadata::getCreationCount( std::string const& name ) const
	{
		return mOwner->getCreationCount( name );
	}

}