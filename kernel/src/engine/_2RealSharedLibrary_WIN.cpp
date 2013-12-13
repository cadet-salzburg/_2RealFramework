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


#include "_2RealSharedLibrary_WIN.h"
#include "helpers/_2RealPath.h"
#include "helpers/_2RealException.h"

#include <Windows.h>

namespace _2Real
{

	SharedLibraryImpl::SharedLibraryImpl() :
		mHandle( nullptr )
	{
	}

	void SharedLibraryImpl::loadImpl( Path const& path)
	{
		std::string p = path.string();

		if ( mHandle ) throw BundleImportException( path.string() );
		DWORD flags = path.is_absolute() ? LOAD_WITH_ALTERED_SEARCH_PATH : 0x00000000;
		mHandle = LoadLibraryExA( p.c_str(), 0, flags );
		if ( !mHandle )
			throw BundleImportException( path.string() );
	}

	void SharedLibraryImpl::unloadImpl()
	{
		if ( mHandle )
		{
			FreeLibrary( (HMODULE) mHandle );
			mHandle = nullptr;
		}
	}

	bool SharedLibraryImpl::isLoadedImpl() const
	{
		return ( mHandle != nullptr ); 
	}

	void * SharedLibraryImpl::findSymbolImpl( std::string const& name ) const
	{
		if ( mHandle )
			return ( void * ) GetProcAddress( ( HMODULE ) mHandle, name.c_str() );
		else
			return nullptr;
	}

} 
