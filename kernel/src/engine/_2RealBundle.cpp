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
#include "engine/_2RealSharedLibraryMetainfo.h"

namespace _2Real
{

	Bundle::Bundle( std::shared_ptr< const SharedLibraryMetainfo > meta ) :
		enable_shared_from_this< Bundle >(),
		mMetadata( meta ), mUnloadNotifier()
	{
	}

	void Bundle::unload( const long timeout )
	{
		// hold the ptr to the bundle until everything is cleared
		std::shared_ptr< Bundle > tmp = shared_from_this();

		// causes the bundle collection to release the bundle
		// unless the call to unload was issued by the collection itesself
		mUnloadNotifier.notify( shared_from_this() );
		mUnloadNotifier.clear();

		// TODO: kill child blocks etc.
		( void ) ( timeout );

		// this should kill the bundle, finally
		tmp.reset();
	}

	Path const& Bundle::getFilePath() const
	{
		return mMetadata->getFilePath();
	}

}
