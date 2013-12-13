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


#include "_2RealSharedLibrary_UNIX.h"
#include "helpers/_2RealException.h"

namespace _2Real
{
	//FastMutex SharedLibraryImpl::_mutex;

	//SharedLibraryImpl::SharedLibraryImpl()
	//{
	//	_handle = 0;
	//}

	//void SharedLibraryImpl::loadImpl(const std::string& path)
	//{
	//	FastMutex::ScopedLock lock(_mutex);
	//
	//	if (_handle) throw LibraryAlreadyLoadedException(path);
	//	_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	//	if (!_handle)
	//	{
	//		const char* err = dlerror();
	//		throw LibraryLoadException(err ? std::string(err) : path);
	//	}
	//	_path = path;
	//}

	//void SharedLibraryImpl::unloadImpl()
	//{
	//	FastMutex::ScopedLock lock(_mutex);
	//
	//	if (_handle)
	//	{
	//		dlclose(_handle);
	//		_handle = 0;
	//	}
	//}

	//bool SharedLibraryImpl::isLoadedImpl() const
	//{
	//	return _handle != 0; 
	//}

	//void* SharedLibraryImpl::findSymbolImpl(const std::string& name) const
	//{
	//	FastMutex::ScopedLock lock(_mutex);
	//
	//	void* result = 0;
	//	if (_handle)
	//	{
	//		result = dlsym(_handle, name.c_str());
	//	}
	//	return result;
	//}
	//
	//

}
