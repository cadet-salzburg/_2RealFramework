
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
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealBundleManager.h"
#include "engine/_2RealSystem.h"
#include "engine/_2RealEngineImpl.h"

#include "_2RealConstants.h"

namespace _2Real
{

	Bundle::Bundle( EngineImpl *engine, std::shared_ptr< const BundleMetadata > meta ) :
		NonCopyable< Bundle >(),
		Identifiable< Bundle >( Ids(), meta->getName() ),
		mEngineImpl( engine ),
		mBundleMetadata( meta )
	{
	}

	std::shared_ptr< const BundleMetadata > Bundle::getBundleMetadata() const
	{
		return mBundleMetadata;
	}

	std::string const& Bundle::getAbsPath() const
	{
		return mBundleMetadata->getInstallDirectory();
	}

	void Bundle::unload( const long timeout )
	{
		// kill all blocks ( regular, context )  where this bundle is involved
		mEngineImpl->getBlockManager()->destroyBlocks( this );
		// unload the bundle -> kills this object
		mEngineImpl->getBundleManager()->unloadBundle( this, timeout );
	}

	std::shared_ptr< FunctionBlock > Bundle::getContextBlockInstance()
	{
		std::shared_ptr< FunctionBlock > context;
		if ( mBundleMetadata->exportsContext() )
		{
			context = mEngineImpl->getBlockManager()->findContextBlockInstance( this );
			if ( context.get() == nullptr )
			{
				context = mEngineImpl->getBundleManager()->createContextBlockInstance( this );
				mEngineImpl->getBlockManager()->addBlockInstance( this, context );
			}
		}

		return context;
	}

	std::shared_ptr< FunctionBlock > Bundle::createFunctionBlockInstance( std::string const& blockName )
	{
		// name
		const unsigned int cnt = mBundleMetadata->getCreationCount( blockName );
		std::ostringstream name;
		name << blockName << "_" << cnt;

		std::shared_ptr< const BlockMetadata > blockMetadata = mBundleMetadata->getFunctionBlockMetadata( blockName );
		if ( blockMetadata->needsContext() )
		{
			std::shared_ptr< FunctionBlock > context = getContextBlockInstance();
		}

		// create block & return
		std::shared_ptr< FunctionBlock > block = mEngineImpl->getBundleManager()->createBlockInstance( this, blockName, name.str() );
		mEngineImpl->getBlockManager()->addBlockInstance( this, block );
		return block;
	}
}
