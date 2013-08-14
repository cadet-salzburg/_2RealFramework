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

#include "helpers/_2RealConstants.h"

namespace _2Real
{
	const std::string Constants::BasicTypename = "BasicType";
	const std::string Constants::FrameworkTypename = "FrameworkType";
	const std::string Constants::ContextBlockName = "Context";

#ifdef _WIN64
	#ifndef _DEBUG
		const std::string Constants::SharedLibrarySuffix = "_64.dll";
	#else
		const std::string Constants::SharedLibrarySuffix = "_64d.dll";
	#endif
#elif _WIN32
	#ifndef _DEBUG
		const std::string Constants::SharedLibrarySuffix = "_32.dll";
	#else
		const std::string Constants::SharedLibrarySuffix = "_32d.dll";
	#endif
#elif _UNIX
	#ifndef _DEBUG
		const std::string Constants::SharedLibrarySuffix = ".so";
	#else
		const std::string Constants::SharedLibrarySuffix = "_d.so";
	#endif
#else
	#ifndef _DEBUG
		const std::string Constants::SharedLibrarySuffix = ".dylib";
	#else
		const std::string Constants::SharedLibrarySuffix = "_d.dylib";
	#endif
#endif

}