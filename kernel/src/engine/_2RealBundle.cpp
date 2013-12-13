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
//#include "engine/_2RealBlockMetadata.h"
//#include "engine/_2RealBundleMetadata.h"
#include "engine/_2RealBundleCollection.h"
//#include "engine/_2RealSystem.h"
//#include "engine/_2RealEngineImpl.h"
//
//#include "helpers/_2RealConstants.h"

namespace _2Real
{
	Bundle::Bundle( std::shared_ptr< BundleCollection > collection, std::shared_ptr< const BundleMetadata > meta ) :
		enable_shared_from_this< Bundle >(),
		mBundleCollection( collection ),
		mBundleMetadata( meta )
	{
	}

	void Bundle::unload( const long timeout )
	{
		// kill all blocks ( regular, context )  where this bundle is involved
		// mEngineImpl->getBlockManager()->destroyBlocks( shared_from_this() );
		// unload the bundle -> kills this object
		mBundleCollection->unloadBundle( shared_from_this(), timeout );
	}
}
