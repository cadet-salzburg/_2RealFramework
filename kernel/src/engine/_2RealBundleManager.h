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

#include "engine/_2RealBundleLoader.h"
#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class Bundle;
	class FunctionBlock;
	class EngineImpl;

	class BundleManager
	{
	
	public:

		static const std::string						sContextBlock;

		BundleManager( EngineImpl *engine );
		~BundleManager();

		void clear();
		std::shared_ptr< Bundle > 						loadBundle( std::string const& );
		std::shared_ptr< Bundle > 						findBundleByPath( std::string const& ) const;
		std::shared_ptr< FunctionBlock >				createContextBlockInstance( Bundle * );
		std::shared_ptr< FunctionBlock >				createBlockInstance( Bundle *, std::string const&, std::string const& );
		void											unloadBundle( Bundle *, const long timeout );
		std::string										getBundleDirectory() const;

	private:

		typedef std::map< std::string, std::shared_ptr< Bundle > >					Bundles;
		typedef std::map< std::string, std::shared_ptr< Bundle > >::iterator		BundleIterator;
		typedef std::map< std::string, std::shared_ptr< Bundle > >::const_iterator	BundleConstIterator;

		const Poco::Path								makeAbsolutePath( Poco::Path const& path ) const;

		EngineImpl										*const mEngineImpl;
		BundleLoader									mBundleLoader;
		Poco::Path										mBundleDirectory;
		Bundles											mBundles;

	};
}