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

#include "Poco/Path.h"

namespace _2Real
{

	Engine & Engine::instance()
	{
		static SingletonHolder< Engine > holder;
		return holder.instance();
	}

	Engine::Engine() :
		m_Engine(EngineImpl::instance())
	{
	}

	Engine::~Engine()
	{
	}

	void Engine::setBaseDirectory(std::string const& directory)
	{
		m_Engine.setBaseDirectory(Poco::Path(directory));
	}

	//const bool Engine::isLoaded(std::string const& className) const
	//{
	//	return m_Engine.isLibraryLoaded(className);
	//}
	//
	//const bool Engine::isSingleton(std::string const& className) const
	//{
	//	return m_Engine.isSingleton(className);
	//}

	//const bool Engine::isSingleton(Identifier const& id) const
	//{
	//	return m_Engine.isSingleton(id);
	//}

	const std::list< std::string > Engine::load(std::string const& path)
	{
		return m_Engine.loadLibrary(Poco::Path(path));
	}

	const bool Engine::isLoaded(std::string const& libraryPath) const
	{
		return false; //m_Engine.isLoaded(Poco::Path(libraryPath));
	}

	//const Identifier Engine::createInstance(std::string const& idName, std::string const& className)
	//{
	//}

	//const std::string Engine::getPluginInfo(Identifier const& pluginId)
	//{
	//	return m_Engine.getInfo(pluginId);
	//}

	//const Identifier System::loadPlugin(std::string const& name, std::string const& classname, std::string const& path)
	//{
	//	std::string idName = validateName(name);
	//	unique(idName);

	//	const Identifier id = m_EngineImpl.loadPlugin(name, classname, m_Id);
	//	m_Lookup.insert(NamedId(idName, id));
	//	return id;
	//}

}