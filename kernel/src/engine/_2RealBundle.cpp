
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

#include "engine/_2RealBundle.h"
#include "engine/_2RealBundleManager.h"
#include "engine/_2RealSystem.h"
#include "engine/_2RealEngineImpl.h"

#include <sstream>

namespace _2Real
{

	Bundle::Bundle( EngineImpl *engine, app::BundleInfo const& info ) :
		NonCopyable< Bundle >(),
		Identifiable< Bundle >( Ids(), info.name ),
		mEngineImpl( engine ),
		mBundleInfo( info )
	{
	}

	app::BundleInfo const& Bundle::getBundleInfo() const
	{
		return mBundleInfo;
	}

	std::string const& Bundle::getAbsPath() const
	{
		return mBundleInfo.directory;
	}

	void Bundle::unload( const long timeout )
	{
		// kill all blocks ( regular, context )  where this bundle is involved
		mEngineImpl->getBlockManager()->destroyBlocks( this );
		// unload the bundle -> kills this object
		mEngineImpl->getBundleManager()->unloadBundle( this, timeout );
	}

	std::shared_ptr< FunctionBlock > Bundle::getContextBlock()
	{
		return mEngineImpl->getBlockManager()->getContextBlock( this );
	}

	std::shared_ptr< FunctionBlock > Bundle::createBlockInstance( std::string const& blockName )
	{
		// name
		const unsigned int cnt = mEngineImpl->getBlockManager()->getBlockInstanceCount( this, blockName );
		std::ostringstream name;
		name << blockName << "_" << cnt;

		// get context block, if needed ( otherwise null is returned )
		std::shared_ptr< FunctionBlock > context = mEngineImpl->getBundleManager()->createContextBlockConditionally( this, blockName );
		if ( context.get() != nullptr )
			mEngineImpl->getBlockManager()->addContextBlockInstance( this, context );

		// create block & return
		std::shared_ptr< FunctionBlock > block = mEngineImpl->getBundleManager()->createBlockInstance( this, blockName, name.str() );
		mEngineImpl->getBlockManager()->addRegularBlockInstance( this, block );
		return block;
	}
}
