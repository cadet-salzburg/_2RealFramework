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

#include "engine/_2RealSharedServiceMetainfo.h"
#include "engine/_2RealSharedServiceOutletMetainfo.h"
#include "engine/_2RealSharedServiceInletMetainfo.h"
#include "engine/_2RealDefaultPolicy.h"
#include "engine/_2RealAbstractSharedServiceFactory.h"
#include "helpers/_2RealException.h"

namespace _2Real
{

	SharedServiceMetainfo::SharedServiceMetainfo( std::string const& name ) :
		std::enable_shared_from_this< SharedServiceMetainfo >(),
		mName( name ),
		mDescription( "" ),
		mIsSingleton( false )
	{
	}

	SharedServiceMetainfo::~SharedServiceMetainfo()
	{
	}

	void SharedServiceMetainfo::setSingleton( const bool isSingleton )
	{
		mIsSingleton = isSingleton;
	}

	void SharedServiceMetainfo::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void SharedServiceMetainfo::setDependencies( std::vector< std::string > const& dependencies )
	{
		mDependencies = dependencies;
	}

	void SharedServiceMetainfo::setDefaultUpdatePolicy( std::shared_ptr< DefaultPolicy > policy )
	{
		mDefaultUpdatePolicy = policy;
	}

	void SharedServiceMetainfo::setDefaultSetupPolicy( std::shared_ptr< DefaultPolicy > policy )
	{
		mDefaultSetupPolicy = policy;
	}

	std::shared_ptr< SharedServiceInletMetainfo > SharedServiceMetainfo::createInlet( std::string const& name )
	{
		if ( mInlets.find( name ) != mInlets.end() )
		{
			std::ostringstream msg;
			msg << "an inlet named " << name << " was already added to block " << mName << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< SharedServiceInletMetainfo > result( new SharedServiceInletMetainfo( name ) );
		mInlets[ name ] = result;
		return result;
	}

	std::shared_ptr< SharedServiceOutletMetainfo > SharedServiceMetainfo::createOutlet( std::string const& name )
	{
		if ( mOutlets.find( name ) != mOutlets.end() )
		{
			std::ostringstream msg;
			msg << "an outlet named " << name << " was already added to block " << mName << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< SharedServiceOutletMetainfo > result( new SharedServiceOutletMetainfo( name ) );
		mOutlets[ name ] = result;
		return result;
	}

	std::shared_ptr< SharedServiceInletMetainfo > SharedServiceMetainfo::createParameter( std::string const& name )
	{
		if ( mParameters.find( name ) != mParameters.end() )
		{
			std::ostringstream msg;
			msg << "a parameter named " << name << " was already added to block " << mName << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		std::shared_ptr< SharedServiceInletMetainfo > result( new SharedServiceInletMetainfo( name ) );
		mParameters[ name ] = result;
		return result;
	}

	bool SharedServiceMetainfo::isSingleton() const
	{
		return mIsSingleton;
	}

	std::string const& SharedServiceMetainfo::getName() const
	{
		return mName;
	}

	std::string const& SharedServiceMetainfo::getDescription() const
	{
		return mDescription;
	}

	void SharedServiceMetainfo::getDependencies( std::vector< std::string > &dependencies ) const
	{
		for ( auto it : mDependencies )
			dependencies.push_back( it );
	}

	void SharedServiceMetainfo::getInletMetainfos( std::vector< std::shared_ptr< const SharedServiceInletMetainfo > > &inlets ) const
	{
		for ( auto it : mInlets )
			inlets.push_back( it.second );
	}

	void SharedServiceMetainfo::getOutletMetainfos( std::vector< std::shared_ptr< const SharedServiceOutletMetainfo  > > &outlets ) const
	{
		for ( auto it : mOutlets )
			outlets.push_back( it.second );
	}

	void SharedServiceMetainfo::getParameterMetainfos( std::vector< std::shared_ptr< const SharedServiceInletMetainfo  > > &parameters ) const
	{
		for ( auto it : mParameters )
			parameters.push_back( it.second );
	}

	void SharedServiceMetainfo::getDefaultSetupPolicy( std::vector< std::vector< std::string > > &policy ) const
	{
		policy = mDefaultSetupPolicy->getStringRep();
	}

	void SharedServiceMetainfo::getDefaultUpdatePolicy( std::vector< std::vector< std::string > > &policy ) const
	{
		policy = mDefaultUpdatePolicy->getStringRep();
	}

	void SharedServiceMetainfo::setFactory( std::shared_ptr< AbstractSharedServiceFactory > factory )
	{
		mFactory = factory;
	}

	std::shared_ptr< const AbstractSharedServiceFactory > SharedServiceMetainfo::getFactory() const
	{
		return mFactory;
	}
}