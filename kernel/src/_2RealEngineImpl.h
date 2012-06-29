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

#include "_2RealPoco.h"

#include <string>

namespace _2Real
{

	namespace app
	{
		class BundleHandle;
	};

	class Logger;
	class Timer;
	class Typetable;
	class ThreadPool;
	class BundleManager;
	class IdCounter;
	class SystemBlock;
	class BlockIdentifier;
	class BundleIdentifier;
	
	class EngineImpl
	{

		template< typename T >
		friend class SingletonHolder;

	public:

		static EngineImpl & instance();

		// TODO: merge public interfaces with interface of engine impl ?
		Logger&							getLogger();
		Timer &							getTimer();
		Typetable const&				getTypetable() const;
		BundleManager &					getBundleManager();
		ThreadPool &					getThreadPool();
		SystemBlock &					getSystemBlock();

		// retruns time in millis since creation
		const long						getElapsedTime() const;

		BlockIdentifier					createBlockId( std::string const& name );
		BundleIdentifier				createBundleId( std::string const& name );

		void							setBaseDirectory( std::string const& directory );
		app::BundleHandle				loadLibrary( std::string const& libraryPath );
		//const bool					isLibraryLoaded( Poco::Path const& path ) const;

	private:

		EngineImpl();
		~EngineImpl();

		// whatever you do. do not change the ordering of member variables here!
		// ( unless you absolutely have to, in which case, good luck )

		Logger					*m_Logger;
		Timer					*m_Timer;
		Typetable				*m_Typetable;
		ThreadPool				*m_ThreadPool;
		BundleManager			*m_BundleManager;
		IdCounter				*m_IdCounter;
		SystemBlock				*m_SystemBlock;
		Poco::Timestamp			m_Timestamp;

	};

}