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

#include "_2RealEngineData.h"

#include <string>
#include <list>

#include "Poco/Path.h"

#ifdef _2REAL_WINDOWS
	#include "vld.h"
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

namespace _2Real
{

	class Identifier;
	class ThreadPool;
	class PluginPool;
	class ServiceFactory;
	class Typetable;
	class Timer;
	
	class EngineImpl
	{

	public:

		static EngineImpl & instance();

		Timer &							getTimer();
		Timer const&					getTimer() const;
		Typetable &						getTypetable();
		Typetable const&				getTypetable() const;
		PluginPool &					getPluginPool();
		PluginPool const&				getPluginPool() const;
		ThreadPool &					getThreadPool();
		ThreadPool const&				getThreadPool() const;

		void							setBaseDirectory(Poco::Path const& path);
		std::list< std::string >		loadLibrary(Poco::Path const& path);
		const bool						isLibraryLoaded(Poco::Path const& path) const;
		const std::string				getInfoString(std::string const& className, Poco::Path const& libraryPath) const;
		const std::string				getInfoString(Identifier const& pluginId) const;
		const bool						canCreate(std::string const& className, Poco::Path const& libraryPath) const;
		const bool						isSingleton(std::string const& className, Poco::Path const& libraryPath) const;
		const bool						isSetUp(Identifier const& pluginId) const;
		const Identifier				createPlugin(std::string const& className, Poco::Path const& libraryPath);
		const Identifier				createPlugin(std::string const& idName, std::string const& className, Poco::Path const& libraryPath);
		const Identifier				pluginInstance(std::string const& className, Poco::Path const& libraryPath);
		void							setPluginValue(Identifier const& pluginId, std::string const& paramName, EngineData const& value);
		const EngineData				getPluginValue(Identifier const& pluginId, std::string const& paramName) const;
		void							setupPlugin(Identifier const& pluginId);
		const Identifier				getPluginIdentifier(std::string const& idName) const;
		const std::list< std::string >	getExportingLibs(std::string const& className);

	private:

		template< typename T >
		friend class SingletonHolder;

		EngineImpl();
		~EngineImpl();

		Timer					*m_Timer;
		ThreadPool				*m_Threads;
		ServiceFactory			*m_Factory;
		PluginPool				*m_Plugins;
		Typetable				*m_Types;

	};

}