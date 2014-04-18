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

#include "common/_2RealBoost.h"
#include "common/_2RealException.h"
#include "common/_2RealConstants.h"

#include "engine/_2RealSharedLibrary.h"
#include "engine/_2RealBundleMetainfoImpl.h"
#include "engine/_2RealBundleImpl.h"
#include "engine/_2RealBundleCollection.h"

namespace _2Real
{
	class BundlePtrCmp : public std::unary_function< bool, std::pair< boost::signals2::connection, std::shared_ptr< const BundleImpl > > >
	{

	public:

		explicit BundlePtrCmp( std::shared_ptr< const BundleImpl > obj ) : mBaseline( obj )
		{
		}

		bool operator()( std::pair< boost::signals2::connection, std::shared_ptr< const BundleImpl > > other ) const
		{
			return mBaseline.get() == other.second.get();
		}

	private:

		std::shared_ptr< const BundleImpl > mBaseline;

	};

	class BundlePathCmp : public std::unary_function< bool, std::pair< boost::signals2::connection, std::shared_ptr< const BundleImpl > > >
	{

	public:

		explicit BundlePathCmp( const Path path ) : mBaseline( path )
		{
		}

		bool operator()( std::pair< boost::signals2::connection, std::shared_ptr< const BundleImpl > > other ) const
		{
			return mBaseline == other.second->getFilePath();
		}

	private:

		Path mBaseline;

	};

	/////////////////////////////////////////////////////////////////////////////////////

	BundleCollection::BundleCollection( std::shared_ptr< TypeCollection > registry )
	{
		updateBundleDirectory();
	}

	BundleCollection::~BundleCollection()
	{
		clear( 1000 );
	}

	void BundleCollection::clear( const unsigned long timeout )
	{
		for ( auto it : mBundles )
		{
			it.first.disconnect();
			it.second->unload( timeout );
		}

		mBundles.clear();
	}

	Path BundleCollection::getBundleDirectory() const
	{
		return mBundleDirectory;
	}

	void BundleCollection::updateBundleDirectory()
	{
		char * dir = std::getenv( Constants::BundleEnvName.c_str() );
		if ( nullptr != dir )
			mBundleDirectory = Path( dir );

		// TODO: exception? or some reasonable default value
	}

	std::pair< std::shared_ptr< BundleImpl >, std::shared_ptr< const BundleMetainfoImpl > > BundleCollection::loadBundle( Path const& path, std::shared_ptr< TypeCollection > types )
	{
		Path absPath = mBundleDirectory / path;

		auto it = std::find_if( mBundles.begin(), mBundles.end(), BundlePathCmp( absPath ) );
		if ( it != mBundles.end() )
		{
			std::ostringstream msg;
			msg << "shared library " << absPath.string() << " is already loaded";
			throw AlreadyExists( msg.str() );
		}

		// may throw
		std::shared_ptr< SharedLibrary > lib( new SharedLibrary( path ) );

		std::shared_ptr< BundleMetainfoImpl > bundleMetainfo = BundleMetainfoImpl::make( lib, path, types );

		if ( !bundleMetainfo.get() )
		{
			std::ostringstream msg;
			msg << "shared library was found, but is not providing all necessary functions" << std::endl;
			throw BundleImportFailure( msg.str() );
		}

		std::shared_ptr< BundleImpl > bundle = BundleImpl::createFromMetainfo( bundleMetainfo, lib, absPath );

		boost::signals2::connection con = bundle->registerToUnload( std::bind( &BundleCollection::bundleUnloaded, this, std::placeholders::_1 ) );

		auto result = std::make_pair( bundle, bundleMetainfo );

		mBundles.push_back( std::make_pair( con, bundle ) );
		return result;
	}

	void BundleCollection::bundleUnloaded( std::shared_ptr< const BundleImpl > bundle )
	{
		auto it = std::find_if( mBundles.begin(), mBundles.end(), BundlePtrCmp( bundle ) );
		if ( it != mBundles.end() )
			mBundles.erase( it );
	}

}
