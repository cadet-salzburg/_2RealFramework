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

/**
*	annoying & ultimately harmless warning about
*	template expressions that are longer than 255 bytes
*/
#pragma warning(disable:4503)

#include <string>

#ifdef _2REAL_WINDOWS
	//#include "vld.h"
	#ifndef _DEBUG
		#define shared_library_suffix ".dll"
	#else
		#define shared_library_suffix "_d.dll"
	#endif
#elif _2REAL_UNIX
	#ifndef _DEBUG
		#define shared_library_suffix ".so"
	#else
		#define shared_library_suffix "_d.so"
	#endif
#elif _2REAL_MAC
	#ifndef _DEBUG
		#define shared_library_suffix ".dylib"
	#else
		#define shared_library_suffix "_d.dylib"
	#endif
#endif

namespace Poco
{
	class Path;
}

namespace _2Real
{

	class BundleIdentifier;
	class ThreadPool;
	class BundleManager;
	class Typetable;
	class Timer;
	class Logger;
	
	class EngineImpl
	{

	public:

		static EngineImpl & instance();

		Logger&							getLogger();
		Timer &							getTimer();
		Timer const&					getTimer() const;
		Typetable &						getTypetable();
		Typetable const&				getTypetable() const;
		BundleManager &					getPluginPool();
		BundleManager const&			getPluginPool() const;
		ThreadPool &					getThreadPool();
		ThreadPool const&				getThreadPool() const;

		void							setBaseDirectory( Poco::Path const& path );
		const BundleIdentifier			loadLibrary( Poco::Path const& path );
		const bool						isLibraryLoaded( Poco::Path const& path ) const;
		const std::string				getInfoString( BundleIdentifier const& bundleId ) const;

	private:

		template< typename T >
		friend class SingletonHolder;

		EngineImpl();
		~EngineImpl();

		Logger					*m_Logger;
		Timer					*m_Timer;
		Typetable				*m_Types;
		ThreadPool				*m_Threads;
		BundleManager			*m_Plugins;

	};

}