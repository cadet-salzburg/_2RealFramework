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

#include "_2RealEngine.h"
#include "_2RealEngineImpl.h"
#include "_2RealIdentifier.h"
#include "_2RealSingletonHolder.h"
#include "_2RealHelpers.h"

#include "Poco/Path.h"

namespace _2Real
{

	Engine & Engine::instance()
	{
		static SingletonHolder< Engine > holder;
		return holder.instance();
	}

	Engine::Engine() :
		m_EngineImpl(EngineImpl::instance())
	{
	}

	Engine::Engine(Engine const& src) :
		m_EngineImpl(EngineImpl::instance())
	{
	}

	Engine::~Engine()
	{
	}

	void Engine::setBaseDirectory(std::string const& directory)
	{
		m_EngineImpl.setBaseDirectory(Poco::Path(directory));
	}

	const std::list< std::string > Engine::load(std::string const& path)
	{
		return m_EngineImpl.loadLibrary(Poco::Path(path));
	}

	const bool Engine::isLoaded(std::string const& libraryPath) const
	{
		return m_EngineImpl.isLibraryLoaded(Poco::Path(libraryPath));
	}

	const std::string Engine::getInfoString(std::string const& className, std::string const& libraryPath)
	{
		return m_EngineImpl.getInfoString(className, Poco::Path(libraryPath));
	}

	const bool Engine::canCreate(std::string const& className, std::string const& libraryPath) const
	{
		return m_EngineImpl.canCreate(className, Poco::Path(libraryPath));
	}

	const bool Engine::isSingleton(std::string const& className, std::string const& libraryPath) const
	{
		return m_EngineImpl.isSingleton(className, Poco::Path(libraryPath));
	}

	const Identifier Engine::createInstance(std::string const& idName, std::string const& className, std::string const& libraryPath)
	{
		return m_EngineImpl.createPlugin(idName, className, Poco::Path(libraryPath));
	}

	const Identifier Engine::getInstance(std::string const& className, std::string const& libraryPath)
	{
		return m_EngineImpl.pluginInstance(className, Poco::Path(libraryPath));
	}

	const bool Engine::isSetUp(Identifier const& pluginId) const
	{
		return m_EngineImpl.isSetUp(pluginId);
	}

	const EngineData Engine::getValueInternal(Identifier const& pluginId, std::string const& paramName) const
	{
		return m_EngineImpl.getPluginValue(pluginId, paramName);
	}

	void Engine::setValueInternal(Identifier const& pluginId, std::string const& paramName, EngineData const& value)
	{
		m_EngineImpl.setPluginValue(pluginId, paramName, value);
	}

	const Identifier Engine::getIdentifier(std::string const& idName) const
	{
		return m_EngineImpl.getPluginIdentifier(idName);
	}

	void Engine::setup(Identifier const& pluginId)
	{
		m_EngineImpl.setupPlugin(pluginId);
	}

}