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

#include "_2RealEngineImpl.h"
#include "_2RealBundleIdentifier.h"
#include "_2RealBlockIdentifier.h"
#include "_2RealSingletonHolder.h"
#include "_2RealTimer.h"
#include "_2RealTypetable.h"
#include "_2RealBundleManager.h"
#include "_2RealThreadPool.h"
#include "_2RealLogger.h"
#include "_2RealSystemImpl.h"
#include "_2RealIdCounter.h"

#include "_2RealBlob.h"
#include "_2RealImageT.h"
#include "_2RealEnum.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	EngineImpl & EngineImpl::instance()
	{
		static SingletonHolder< EngineImpl > holder;
		return holder.instance();
	}

	EngineImpl::EngineImpl() :
		m_Logger( new Logger( "Logfile.txt" ) ),
		m_Timer( new Timer() ),
		m_Types( new Typetable(*this) ),
		m_Threads( new ThreadPool(15, 0, "2real engine threadpool") ),
		m_Plugins( new BundleManager() ),
		m_IdCounter( new IdCounter() )
	{
		m_Types->registerType< char >("char");
		m_Types->registerType< unsigned char >("unsigned char");
		m_Types->registerType< short >("short");
		m_Types->registerType< unsigned short >("unsigned short");
		m_Types->registerType< int >("int");
		m_Types->registerType< unsigned int >("unsigned int");
		m_Types->registerType< long >("long");
		m_Types->registerType< unsigned long >("unsigned long");
		m_Types->registerType< float >("float");
		m_Types->registerType< double >("double");
		m_Types->registerType< bool >("bool");
		m_Types->registerType< std::string >("string");

		m_Types->registerType< StringEnumeration >( "enum_string" );

		m_Types->registerType< ImageT < char > >("img_char");
		m_Types->registerType< ImageT < unsigned char > >("img_uchar");
		m_Types->registerType< ImageT < short > >("img_short");
		m_Types->registerType< ImageT < unsigned short > >("img_ushort");
		m_Types->registerType< ImageT < int > >("img_int");
		m_Types->registerType< ImageT < unsigned int > >("img_uint");
		m_Types->registerType< ImageT < long > >("img_long");
		m_Types->registerType< ImageT < unsigned long > >("img_ulong");
		m_Types->registerType< ImageT < float > >("img_float");
		m_Types->registerType< ImageT < double > >("img_double");

		m_Timestamp.update();
		m_Threads->registerTimeListener(*m_Timer);
	}

	EngineImpl::~EngineImpl()
	{
		try
		{
			delete m_IdCounter;
			delete m_Plugins;

			m_Threads->unregisterTimeListener( *m_Timer );
			m_Threads->clear();

			delete m_Types;
			delete m_Threads;
			delete m_Timer;

			m_Logger->stop();
			delete m_Logger;
		}
		catch (std::exception &e)
		{
#ifdef _DEBUG
			std::cout << e.what() << std::endl;
#endif
		}
	}

	const long EngineImpl::getElapsedTime() const
	{
		return static_cast< long >( m_Timestamp.elapsed() );
	}

	Poco::Timestamp const& EngineImpl::getTimestamp() const
	{
		return m_Timestamp;
	}

	BlockIdentifier EngineImpl::createBlockId( std::string const& name )
	{
		BlockIdentifier id;
		id.m_Id = m_IdCounter->getId();
		id.m_Name = name;

		return id;
	}

	BundleIdentifier EngineImpl::createBundleId( std::string const& name )
	{
		BundleIdentifier id;
		id.m_Id = m_IdCounter->getId();
		id.m_Name = name;

		return id;
	}

	Logger& EngineImpl::getLogger()
	{
		return *m_Logger;
	}

	Timer & EngineImpl::getTimer()
	{
		return *m_Timer;
	}

	Timer const& EngineImpl::getTimer() const
	{
		return *m_Timer;
	}

	Typetable & EngineImpl::getTypetable()
	{
		return *m_Types;
	}

	Typetable const& EngineImpl::getTypetable() const
	{
		return *m_Types;
	}

	BundleManager & EngineImpl::getPluginPool()
	{
		return *m_Plugins;
	}

	BundleManager const& EngineImpl::getPluginPool() const
	{
		return *m_Plugins;
	}

	ThreadPool & EngineImpl::getThreadPool()
	{
		return *m_Threads;
	}

	ThreadPool const& EngineImpl::getThreadPool() const
	{
		return *m_Threads;
	}

	void EngineImpl::setBaseDirectory(Poco::Path const& directory)
	{
		m_Plugins->setBaseDirectory(directory);
	}

	const BundleIdentifier EngineImpl::loadLibrary(Poco::Path const& path)
	{
		return m_Plugins->loadLibrary(path);
	}

	const bool EngineImpl::isLibraryLoaded(Poco::Path const& path) const
	{
		return m_Plugins->isLibraryLoaded(path);
	}

	const std::string EngineImpl::getInfoString( BundleIdentifier const& bundleId ) const
	{
		return m_Plugins->getInfoString( bundleId );
	}

	BundleData const& EngineImpl::getBundleData( BundleIdentifier const& bundleId ) const
	{
		return m_Plugins->getBundleData( bundleId );
	}

	BlockData const& EngineImpl::getBlockData( BundleIdentifier const& bundleId, std::string const& blockName ) const
	{
		return m_Plugins->getBlockData( bundleId, blockName );
	}

	//const Identifier EngineImpl::getPluginIdentifier( std::string const& idName ) const
	//{
	//	return m_Plugins->getIdentifier(idName);
	//}

}