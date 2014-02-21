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

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealVersion.h"
#include "helpers/_2RealPath.h"

namespace _2Real
{

	class SharedTypeMetainfo;
	class SharedServiceMetainfo;

	class SharedLibraryMetainfo : public std::enable_shared_from_this< SharedLibraryMetainfo >
	{

	public:

		SharedLibraryMetainfo( Path const& absPath );
		~SharedLibraryMetainfo();

		std::string const&		getName() const;
		Path const&				getFilePath() const;
		std::string const&		getDescription() const;
		std::string const&		getAuthor() const;
		std::string const&		getContact() const;
		std::string const&		getCategory() const;
		Version const&			getVersion() const;

		void					setDescription( std::string const& );
		void					setAuthor( std::string const& );
		void					setContact( std::string const& );
		void					setCategory( std::string const& );
		void					setVersion( Version const& );

		void					getServiceMetainfos( std::vector< std::shared_ptr< const SharedServiceMetainfo > > &infos ) const;
		void					getTypeMetainfos( std::vector< std::shared_ptr< const SharedTypeMetainfo > > &infos ) const;

		std::shared_ptr< SharedServiceMetainfo > createService( std::string const& name, const bool isSingleton );
		std::shared_ptr< SharedTypeMetainfo > createType( std::string const& name );

		bool performExport();

	private:

		// called by perform export
		//bool isBasicDataOk() const;
		//bool isServiceDataOk() const;
		//bool isTypeDataOk() const;

		SharedLibraryMetainfo( SharedLibraryMetainfo const& );
		SharedLibraryMetainfo& operator=( SharedLibraryMetainfo const& );

		// basic info, bundle metainfo provides setters here
		std::string mName;
		std::string mCategory;
		std::string mDescription;
		std::string mAuthor;
		std::string mContact;
		Version mVersion;
		Path mPath;

		typedef std::map< std::string, std::shared_ptr< SharedServiceMetainfo > >	ServiceMetainfos;
		typedef std::map< std::string, std::shared_ptr< SharedTypeMetainfo > >		TypeMetainfos;

		ServiceMetainfos		mServices;
		TypeMetainfos			mTypes;

	};

}