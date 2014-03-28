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

#include "engine/_2RealSharedLibraryMetainfo.h"
#include "engine/_2RealSharedServiceMetainfo.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"
#include "helpers/_2RealException.h"

namespace _2Real
{

	SharedLibraryMetainfo::SharedLibraryMetainfo( Path const& absPath ) :
		std::enable_shared_from_this< SharedLibraryMetainfo >(),
		mName(), mCategory(), mDescription(), mAuthor(), mContact(), mVersion( 0, 0, 0 ), mPath( absPath ),
		mTypes( new TypeCollection )
	{
	}

	SharedLibraryMetainfo::~SharedLibraryMetainfo()
	{
		mServices.clear();
		mTypes->clear();
	}

	std::shared_ptr< SharedServiceMetainfo > SharedLibraryMetainfo::createService( std::string const& name, const bool isSingleton )
	{
		if ( mServices.find( name ) != mServices.end() )
		{
			std::ostringstream msg;
			msg << "a block named " << name << " was already exported by bundle " << mName << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< SharedServiceMetainfo > result( new SharedServiceMetainfo( name, mTypes ) );
		result->setSingleton( isSingleton );
		mServices[ name ] = result;
		return result;
	}

	std::shared_ptr< SharedTypeMetainfo > SharedLibraryMetainfo::createType( std::string const& name )
	{
		//if ( mTypes.find( name ) != mTypes.end() )
		//{
		//	std::ostringstream msg;
		//	msg << "a type named " << name << " was already exported by bundle " << mName << std::endl;
		//	throw AlreadyExistsException( msg.str() );
		//}

		std::shared_ptr< SharedTypeMetainfo > result( new SharedTypeMetainfo( name ) );
		mTypes->addType( result );
		//mTypes[ name ] = result;
		return result;
	}

	bool SharedLibraryMetainfo::performExport()
	{
		bool ok = true;//isBasicDataOk();

		//TypeMetainfos tmpTypes = mTypes;
		//mTypes.clear();

		//for ( auto it : tmpTypes )
		//{
		//	std::string name = it.first;
		//	std::cout << "cloning type metainfo " << name << std::endl;
		//	std::shared_ptr< SharedTypeMetainfo > newInfo( new SharedTypeMetainfo( name ) );
		//	newInfo->cloneFrom( *it.second.get() );
		//	mTypes[ name ] = newInfo;
		//}

		//ok &= isTypeDataOk();

		ServiceMetainfos tmpServices = mServices;
		mServices.clear();

		for ( auto it : tmpServices )
		{
			std::string name = it.first;
			std::cout << "cloning block metainfo " << name << std::endl;
			std::shared_ptr< SharedServiceMetainfo > newInfo( new SharedServiceMetainfo( name ) );
			newInfo->cloneFrom( *it.second.get() );
			mServices[ name ] = newInfo;
		}

		//ok &= isServiceDataOk();

		return ok;
	}

	//bool SharedLibraryMetainfo::isBasicDataOk() const
	//{
	//	return true;
	//}

	//bool SharedLibraryMetainfo::isServiceDataOk() const
	//{
	//	return true;
	//}

	//bool SharedLibraryMetainfo::isTypeDataOk() const
	//{
	//	return true;
	//}

	Path const& SharedLibraryMetainfo::getFilePath() const
	{
		return mPath;
	}

	std::string const& SharedLibraryMetainfo::getName() const
	{
		return mName;
	}

	std::string const& SharedLibraryMetainfo::getDescription() const
	{
		return mDescription;
	}

	std::string const& SharedLibraryMetainfo::getAuthor() const
	{
		return mAuthor;
	}

	std::string const& SharedLibraryMetainfo::getContact() const
	{
		return mContact;
	}

	std::string const& SharedLibraryMetainfo::getCategory() const
	{
		return mCategory;
	}

	Version const& SharedLibraryMetainfo::getVersion() const
	{
		return mVersion;
	}

	void SharedLibraryMetainfo::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void SharedLibraryMetainfo::setAuthor( std::string const& author )
	{
		mAuthor = author;
	}

	void SharedLibraryMetainfo::setContact( std::string const& contact )
	{
		mContact = contact;
	}

	void SharedLibraryMetainfo::setCategory( std::string const& category )
	{
		mCategory = category;
	}

	void SharedLibraryMetainfo::setVersion( Version const& version )
	{
		mVersion = version;
	}

	void SharedLibraryMetainfo::getServiceMetainfos( std::vector< std::shared_ptr< const SharedServiceMetainfo > > &infos ) const
	{
		infos.clear();
		infos.reserve( mServices.size() );
		for ( auto it : mServices )
			infos.push_back( it.second );
	}

	void SharedLibraryMetainfo::getTypeMetainfos( std::vector< std::shared_ptr< const SharedTypeMetainfo > > &infos ) const
	{
		infos.clear();
		//infos.reserve( mTypes.size() );
		//for ( auto it : mTypes )
		//	infos.push_back( it.second );
	}

}