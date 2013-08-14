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
#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealPocoIncludes.h"

namespace _2Real
{
	namespace bundle
	{
		class AbstractBlockCreator;
	}

	class TemplateId;
	class Metainfo;
	class BlockMetadata;
	class TypeMetadata;

	class BundleMetadata
	{

	public:

		BundleMetadata( Metainfo * );

		void						setDescription( std::string const& description );
		void						setAuthor( std::string const& author );
		void						setContact( std::string const& contact );
		void						setInstallDirectory( Poco::Path const& p );
		void						setCategory( std::string const& category );
		void						setVersion( Version const& version );

		std::string const&			getInstallDirectory() const;
		std::string const&			getDescription() const;
		std::string const&			getAuthor() const;
		std::string const&			getContact() const;
		std::string const&			getName() const;
		std::string const&			getCategory() const;
		Version const&				getVersion() const;
		std::shared_ptr< const TemplateId > getIdentifier() const;

		std::shared_ptr< const BlockMetadata >		getFunctionBlockMetadata( std::string const& name ) const;
		std::shared_ptr< const BlockMetadata >		getContextBlockMetadata() const;

		void						getBlockMetadata( std::vector< std::shared_ptr< const BlockMetadata > > & ) const;

		std::shared_ptr< BlockMetadata >	exportFunctionBlock( bundle::AbstractBlockCreator *, std::string const& );
		std::shared_ptr< BlockMetadata >	exportContextBlock( bundle::AbstractBlockCreator * );
		std::shared_ptr< TypeMetadata >		exportCustomType( std::string const& );
	
		bool						exportsContext() const;
		bool						hasContext() const;
		unsigned int				getCreationCount( std::string const& ) const;

	private:

		Metainfo					*const mOwner;

		std::string					mName;
		std::string					mInstallDirectory;
		std::string					mDescription;
		std::string					mAuthor;
		std::string					mContact;
		std::string					mCategory;
		Version						mVersion;

	};
}