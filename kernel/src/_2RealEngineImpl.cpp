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
#include "_2RealIdentifier.h"
#include "_2RealSingletonHolder.h"
#include "_2RealTimer.h"
#include "_2RealTypetable.h"
#include "_2RealServiceFactory.h"
#include "_2RealPluginPool.h"
#include "_2RealThreadPool.h"

#include "_2RealPixelbuffer.h"
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
		m_Timer(new Timer()),
		m_Threads(new ThreadPool(15, 0, "2real engine threadpool")),
		m_Plugins(new PluginPool()),
		m_Factory(new ServiceFactory(*this)),
		m_Types(new Typetable(*this))
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

		/**
		*	our enumeration datatype
		*/
		m_Types->registerType< Enumeration >("enum");

		/**
		*	our image datatype
		*/
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

		/**
		*	this is going to be deleted
		*/
		m_Types->registerType< Buffer2D_float >("buffer2D_float");
		m_Types->registerType< Buffer2D_double >("buffer2D_double");
		m_Types->registerType< Buffer2D_uint >("buffer2D_uint");
		m_Types->registerType< Buffer2D_uchar >("buffer2D_uchar");

		/**
		*	blobs for all primitive datatypes-> feel free to add / delete
		*/
		m_Types->registerType< Blob < char > >("blob_char");
		m_Types->registerType< Blob < unsigned char > >("blob_uchar");
		m_Types->registerType< Blob < short > >("blob_short");
		m_Types->registerType< Blob < unsigned short > >("blob_ushort");
		m_Types->registerType< Blob < int > >("blob_int");
		m_Types->registerType< Blob < unsigned int > >("blob_uint");
		m_Types->registerType< Blob < long > >("blob_long");
		m_Types->registerType< Blob < unsigned long > >("blob_ulong");
		m_Types->registerType< Blob < float > >("blob_float");
		m_Types->registerType< Blob < double > >("blob_double");
		m_Types->registerType< Blob < bool > >("blob_bool");


		m_Threads->registerTimeListener(*m_Timer);
	}

	EngineImpl::~EngineImpl()
	{
		try
		{
			m_Plugins->clear();
			m_Threads->unregisterTimeListener(*m_Timer);
			m_Threads->clear();

			delete m_Plugins;
			delete m_Factory;
			delete m_Types;
			delete m_Threads;
			delete m_Timer;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
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

	PluginPool & EngineImpl::getPluginPool()
	{
		return *m_Plugins;
	}

	PluginPool const& EngineImpl::getPluginPool() const
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

	//const Identifier EngineImpl::createSystem(std::string const& systemName)
	//{
	//	return m_Graphs.createSystemImpl(systemName);
	//}

	//void EngineImpl::clearSystem(Identifier const& systemId)
	//{
	//}

	//void EngineImpl::destroySystem(Identifier const& systemId)
	//{
	//	m_Graphs.destroySystemImpl(systemId);
	//}

	//void EngineImpl::setSystemLogfile(std::string const& filepath, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.setLogfile(filepath);
	//	if (!nirvana.isLoggingEnabled())
	//	{
	//	}
	//}

	//
	void EngineImpl::setBaseDirectory(Poco::Path const& directory)
	{
		m_Plugins->setBaseDirectory(directory);
	}

	//
	std::list< std::string > EngineImpl::loadLibrary(Poco::Path const& path)
	{
		return m_Plugins->loadLibrary(path);
	}

	//
	const bool EngineImpl::isLibraryLoaded(Poco::Path const& path) const
	{
		return m_Plugins->isLibraryLoaded(path);
	}

	//
	const std::string EngineImpl::getInfoString(std::string const& className, Poco::Path const& libraryPath) const
	{
		return m_Plugins->getInfoString(className, libraryPath);
	}

	//
	const std::string EngineImpl::getInfoString(Identifier const& pluginId) const
	{
		if (!pluginId.isPlugin())
		{
			std::ostringstream msg;
			msg << "EngineImpl::getInfoString " << pluginId.name() << " is a " << pluginId.type() << ", plugin expected";
			throw InvalidIdentifierException(msg.str());
		}

		return m_Plugins->getInfoString(pluginId);
	}

	const Identifier EngineImpl::getPluginIdentifier(std::string const& idName) const
	{
		return m_Plugins->getIdentifier(idName);
	}

	const bool EngineImpl::canCreate(std::string const& className, Poco::Path const& libraryPath) const
	{
		return m_Plugins->canCreate(className, libraryPath);
	}

	const bool EngineImpl::isSingleton(std::string const& className, Poco::Path const& libraryPath) const
	{
		return m_Plugins->isSingleton(className, libraryPath);
	}

	const bool EngineImpl::isSetUp(Identifier const& pluginId) const
	{
		return m_Plugins->isSetUp(pluginId);
	}

	const Identifier EngineImpl::createPlugin(std::string const& className, Poco::Path const& libraryPath)
	{
		return m_Plugins->createInstance(className, libraryPath);
	}

	const Identifier EngineImpl::createPlugin(std::string const& idName, std::string const& className, Poco::Path const& libraryPath)
	{
		return m_Plugins->createInstance(idName, className, libraryPath);
	}

	const Identifier EngineImpl::pluginInstance(std::string const& className, Poco::Path const& libraryPath)
	{
		return m_Plugins->getInstance(className, libraryPath);
	}

	void EngineImpl::setupPlugin(Identifier const& id)
	{
		m_Plugins->setup(id);
	}

	void EngineImpl::setPluginValue(Identifier const& pluginId, std::string const& paramName, EngineData const& value)
	{
		if (!pluginId.isPlugin())
		{
			std::ostringstream msg;
			msg << "EngineImpl::setPluginValue " << pluginId.name() << " is a " << pluginId.type() << ", plugin expected";
			throw InvalidIdentifierException(msg.str());
		}

		return m_Plugins->setParameterValue(pluginId, paramName, value);
	}

	const EngineData EngineImpl::getPluginValue(Identifier const& pluginId, std::string const& paramName) const
	{
		if (!pluginId.isPlugin())
		{
			std::ostringstream msg;
			msg << "EngineImpl::setPluginValue " << pluginId.name() << " is a " << pluginId.type() << ", plugin expected";
			throw InvalidIdentifierException(msg.str());
		}

		return m_Plugins->getParameterValue(pluginId, paramName);
	}

	const std::list< std::string > EngineImpl::getExportingLibs(std::string const& className)
	{
		return std::list< std::string >();
	}

	//void EngineImpl::setup(Identifier const& setupAble, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	if (!setupAble.isSetupAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "EngineImpl::setup " << setupAble.name() << " is a " << setupAble.type() << ", plugin or service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.setUp(setupAble);
	//}

	//const Identifier EngineImpl::createService(Identifier const& pluginId, std::string const& serviceName, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	if (!pluginId.isPlugin())
	//	{
	//		std::ostringstream msg;
	//		msg << "EngineImpl::getInfo " << pluginId.name() << " is a " << pluginId.type() << ", plugin expected" << std::endl;
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	return nirvana.createService(pluginId, serviceName);
	//}

	//const Identifier EngineImpl::createService(std::string const& name, Identifier const& pluginId, std::string const& serviceName, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	if (!pluginId.isPlugin())
	//	{
	//		std::ostringstream msg;
	//		msg << "EngineImpl::getInfo " << pluginId.name() << " is a " << pluginId.type() << ", plugin expected" << std::endl;
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	return nirvana.createService(name, pluginId, serviceName);
	//}

	//void EngineImpl::setValue(Identifier const& id, std::string const& paramName, EngineData const& value, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	if (!id.isSetupAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "EngineImpl::setValue " << id.name() << " is a " << id.type() << ", plugin or service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.setValue(id, paramName, value);
	//}

	//void EngineImpl::setValueToString(Identifier const& id, std::string const& paramName, std::string const& value, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	if (!id.isSetupAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "EngineImpl::setValueToString " << id.name() << " is a " << id.type() << ", plugin or service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	EngineData val;
	//	std::string key = nirvana.getParameterKey(id, paramName);
	//	val.create(m_Types.getInitialValueFromKey(key));

	//	std::stringstream stream;
	//	stream << value;
	//	stream >> val;
	//	nirvana.setValue(id, paramName, val);
	//}

	//const EngineData EngineImpl::getValue(Identifier const& id, std::string const& name, Identifier const& systemId) const
	//{
	//	SystemImpl const& nirvana = m_Graphs.getSystemImpl(systemId);

	//	if (!id.isSetupAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "EngineImpl::setValue " << id.name() << " is a " << id.type() << ", plugin or service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	return nirvana.getValue(id, name);
	//}

	//void EngineImpl::setUpdateRate(Identifier const& id, float updatesPerSecond, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!id.isService())
	//	{
	//		std::ostringstream msg;
	//		msg << "set update rate: " << id.name() << " is a " << id.type() << ", service expected" << std::endl;
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.setUpdateRate(id, updatesPerSecond);
	//}

	//void EngineImpl::linkSlots(Identifier const& serviceIn, std::string const& nameIn, Identifier const& serviceOut, std::string const& nameOut, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.linkSlots(serviceIn, nameIn, serviceOut, nameOut);
	//}

	//void EngineImpl::unlinkSlots(Identifier const& idIn, std::string const& nameIn, Identifier const& idOut, std::string const& nameOut, Identifier const& systemId)
	//{
	//}

	//void EngineImpl::clearOutputListeners(Identifier const& idOut, std::string const& nameOut, const bool clearCallbacks, Identifier const& systemId)
	//{
	//}

	//void EngineImpl::clearInputProviders(Identifier const& idIn, std::string const& nameIn, Identifier const& systemId)
	//{
	//}

	//void EngineImpl::registerToStateChange(Identifier const& runnableId, StateChangeCallback callback, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.registerToStateChange(runnableId, callback);
	//}

	//void EngineImpl::unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.unregisterFromStateChange(runnableId, callback);
	//}

	//void EngineImpl::registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.registerToStateChange(runnableId, listener);
	//}

	//void EngineImpl::unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.unregisterFromStateChange(runnableId, listener);
	//}

	//void EngineImpl::registerToException(ExceptionCallback callback, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.registerExceptionCallback(callback);
	//}

	//void EngineImpl::unregisterFromException(ExceptionCallback callback, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.unregisterExceptionCallback(callback);
	//}

	//void EngineImpl::registerToException(IExceptionListener &listener, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.registerExceptionListener(listener);
	//}

	//void EngineImpl::unregisterFromException(IExceptionListener &listener, Identifier const& systemId)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(systemId);

	//	nirvana.unregisterExceptionListener(listener);
	//}

	//void EngineImpl::registerToNewData(Identifier const& service, std::string const& outName, DataCallback callback, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!service.isService())
	//	{
	//		std::ostringstream msg;
	//		msg << "registerToNewData: " << service.name() << " is a " << service.type() << ", service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.registerToNewData(service, outName, callback);
	//}

	//void EngineImpl::unregisterFromNewData(Identifier const& service, std::string const& outName, DataCallback callback, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!service.isService())
	//	{
	//		std::ostringstream msg;
	//		msg << "unregisterFromNewData: " << service.name() << " is a " << service.type() << ", service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.unregisterFromNewData(service, outName, callback);
	//}

	//void EngineImpl::registerToNewData(Identifier const& service, std::string const& outName, IOutputListener &listener, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!service.isService())
	//	{
	//		std::ostringstream msg;
	//		msg << "registerToNewData: " << service.name() << " is a " << service.type() << ", service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}
	//	
	//	nirvana.registerToNewData(service, outName, listener);
	//}

	//void EngineImpl::unregisterFromNewData(Identifier const& service, std::string const& outName, IOutputListener &listener, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!service.isService())
	//	{
	//		std::ostringstream msg;
	//		msg << "unregisterFromNewData: " << service.name() << " is a " << service.type() << ", service expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.unregisterFromNewData(service, outName, listener);
	//}

	//const Identifier EngineImpl::createSequence(std::string const& name, Identifier const& runnableA, Identifier const& runnableB, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!runnableA.isRunAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "createSequence: " << runnableA.name() << " is a " << runnableA.type() << ", runnable (service/sequence/synchronization) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}
	//	if (!runnableB.isRunAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "createSequence: " << runnableB.name() << " is a " << runnableB.type() << ", runnable (service/sequence/synchronization) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	return nirvana.createSequence(name, runnableA, runnableB);
	//}

	//const Identifier EngineImpl::createSynchronization(std::string const& name, Identifier const& runnableA, Identifier const& runnableB, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!runnableA.isRunAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "createSynchronization: " << runnableA.name() << " is a " << runnableA.type() << ", runnable (service/sequence/synchronization) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}
	//	if (!runnableB.isRunAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "createSynchronization: " << runnableB.name() << " is a " << runnableB.type() << ", runnable (service/sequence/synchronization) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	return nirvana.createSynchronization(name, runnableA, runnableB);
	//}

	//void EngineImpl::add(Identifier const& runnable, Identifier const& parent, unsigned int index, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!runnable.isRunAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "insert: " << runnable.name() << " is a " << runnable.type() << ", runnable (service/sequence/synchronization) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}
	//	if (!parent.isContainer())
	//	{
	//		std::ostringstream msg;
	//		msg << "insert: " << parent.name() << " is a " << parent.type() << ", container (sequence/synchronization/system) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.add(runnable, parent, index);
	//}

	//void EngineImpl::append(Identifier const& runnable, Identifier const& parent, Identifier const& system)
	//{
	//	SystemImpl &nirvana = m_Graphs.getSystemImpl(system);

	//	if (!runnable.isRunAble())
	//	{
	//		std::ostringstream msg;
	//		msg << "append: " << runnable.name() << " is a " << runnable.type() << ", runnable (service/sequence/synchronization) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}
	//	if (!parent.isContainer())
	//	{
	//		std::ostringstream msg;
	//		msg << "append: " << parent.name() << " is a " << parent.type() << ", container (sequence/synchronization/system) expected";
	//		throw InvalidIdentifierException(msg.str());
	//	}

	//	nirvana.append(runnable, parent);
	//}


}