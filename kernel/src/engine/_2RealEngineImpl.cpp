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

#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealIdentifier.h"
#include "helpers/_2RealSingletonHolder.h"
#include "engine/_2RealTimer.h"
#include "engine/_2RealTypetable.h"
#include "engine/_2RealBundleManager.h"
#include "engine/_2RealThreadPool.h"
#include "engine/_2RealLogger.h"
#include "engine/_2RealSystem.h"
#include "engine/_2RealIdCounter.h"
#include "app/_2RealBundleHandle.h"
#include "app/_2RealCallbacks.h"
#include "app/_2RealCallbacksInternal.h"

#include "datatypes/_2RealImageT.h"
#include "datatypes/_2RealEnum.h"

#include <sstream>
#include <iostream>

#ifdef _2REAL_WINDOWS
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

using std::string;

namespace _2Real
{

	EngineImpl & EngineImpl::instance()
	{
		static SingletonHolder< EngineImpl > holder;
		return holder.instance();
	}

	EngineImpl::EngineImpl() :
		m_Logger( new Logger( "EngineLog.txt" ) ),
		m_Timer( new Timer() ),
		m_Typetable( new Typetable() ),
		m_ThreadPool( new ThreadPool( 15, 0, "2real threadpool" ) ),
		m_BundleManager( new BundleManager( *this ) ),
		m_IdCounter( new IdCounter() ),
		m_SystemBlock( new System( *this ) )
	{
		m_Typetable->registerType< char >("char");
		m_Typetable->registerType< unsigned char >("unsigned char");
		m_Typetable->registerType< short >("short");
		m_Typetable->registerType< unsigned short >("unsigned short");
		m_Typetable->registerType< int >("int");
		m_Typetable->registerType< unsigned int >("unsigned int");
		m_Typetable->registerType< long >("long");
		m_Typetable->registerType< unsigned long >("unsigned long");
		m_Typetable->registerType< float >("float");
		m_Typetable->registerType< double >("double");
		m_Typetable->registerType< bool >("bool");
		m_Typetable->registerType< std::string >("string");

		m_Typetable->registerType< StringEnumeration >( "enum_string" );

		m_Typetable->registerType< ImageT < char > >("img_char");
		m_Typetable->registerType< ImageT < unsigned char > >("img_uchar");
		m_Typetable->registerType< ImageT < short > >("img_short");
		m_Typetable->registerType< ImageT < unsigned short > >("img_ushort");
		m_Typetable->registerType< ImageT < int > >("img_int");
		m_Typetable->registerType< ImageT < unsigned int > >("img_uint");
		m_Typetable->registerType< ImageT < long > >("img_long");
		m_Typetable->registerType< ImageT < unsigned long > >("img_ulong");
		m_Typetable->registerType< ImageT < float > >("img_float");
		m_Typetable->registerType< ImageT < double > >("img_double");

		// currently, the threadpool uses the timer signal to perform cleanup
		// TODO: probably not a good idea
		m_ThreadPool->registerTimeListener( *m_Timer );

		m_Timestamp.update();
	}

	EngineImpl::~EngineImpl()
	{
		try
		{
			m_SystemBlock->clearAll();
			delete m_SystemBlock;
			delete m_IdCounter;
			delete m_BundleManager;
			m_ThreadPool->unregisterTimeListener( *m_Timer );
			m_ThreadPool->clear();
			delete m_ThreadPool;
			delete m_Typetable;
			delete m_Timer;
			m_Logger->stop();
			delete m_Logger;
		}
		catch ( std::exception &e )
		{
			std::cout << e.what() << std::endl;
		}
	}

	void EngineImpl::clear()
	{
		m_SystemBlock->clearAll();
		m_BundleManager->clear();
	}

	void EngineImpl::clearBlockInstances()
	{
		m_SystemBlock->clearBlockInstances();
	}

	const long EngineImpl::getElapsedTime() const
	{
		return static_cast< long >( m_Timestamp.elapsed() );
	}

	Identifier EngineImpl::createIdentifier( std::string const& name )
	{
		return Identifier( name, m_IdCounter->getId() );
	}

	Logger& EngineImpl::getLogger()
	{
		return *m_Logger;
	}

	Timer & EngineImpl::getTimer()
	{
		return *m_Timer;
	}

	Typetable const& EngineImpl::getTypetable() const
	{
		return *m_Typetable;
	}

	BundleManager & EngineImpl::getBundleManager()
	{
		return *m_BundleManager;
	}

	ThreadPool & EngineImpl::getThreadPool()
	{
		return *m_ThreadPool;
	}

	System & EngineImpl::getSystemBlock()
	{
		return *m_SystemBlock;
	}

	void EngineImpl::setBaseDirectory( string const& directory )
	{
		m_BundleManager->setBaseDirectory( directory );
	}

	app::BundleHandle EngineImpl::loadLibrary( string const& libraryPath )
	{
		string path = libraryPath;
		path.append( shared_library_suffix );
		return m_BundleManager->loadLibrary( path );
	}

	void EngineImpl::registerToException( app::ErrorCallback &callback )
	{
		m_SystemBlock->registerToException( callback );
	}

	void EngineImpl::unregisterFromException( app::ErrorCallback &callback )
	{
		m_SystemBlock->unregisterFromException( callback );
	}

}