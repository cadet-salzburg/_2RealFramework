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
#include "_2RealSystem.h"
#include "_2RealException.h"
#include "_2RealIdentifier.h"

namespace _2Real
{

	System::System(std::string const& name) :
		m_Engine(Engine::instance())
	{
		m_Id = m_Engine.createSystem(name);
	}

	System::System(System const& src) :
		m_Engine(src.m_Engine),
		m_Id(src.m_Id)
	{
	}

	System::~System()
	{
		m_Engine.destroySystem(m_Id);
	}

	void System::destroy()
	{
		m_Engine.destroySystem(m_Id);
	}

	void System::setLogfile(std::string const& file)
	{
		m_Engine.setSystemLogfile(file, m_Id);
	}

	void System::setInstallDirectory(std::string const& directory)
	{
		m_Engine.setSystemDirectory(directory, m_Id);
	}

	const Identifier System::load(std::string const& name, std::string const& classname)
	{
		return m_Engine.load(name, classname, m_Id);
	}

	void System::setup(Identifier const& id)
	{
		m_Engine.setup(id, m_Id);
	}

	const std::string System::getInfo(Identifier const& plugin)
	{
		return m_Engine.getInfo(plugin, m_Id);
	}

	const Identifier System::createService(std::string const& name, Identifier const& plugin, std::string const& service)
	{
		return m_Engine.createService(name, plugin, service, m_Id);
	}

	void System::setUpdateRate(Identifier const& runnable, float const& updatesPerSecond)
	{
		m_Engine.setUpdateRate(runnable, updatesPerSecond, m_Id);
	}

	void System::setValueInternal(Identifier const& id, std::string const& param, EngineData value, type_info const& info)
	{
		m_Engine.setValue(id, param, value, info.name(), m_Id);
	}

	//Identifiers System::getInputSlots(Identifier const& _id)
	//{
	//	try
	//	{
	//		return m_Engine->getInputSlots(_id, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//Identifiers System::getOutputSlots(Identifier const& _id)
	//{
	//	try
	//	{
	//		return m_Engine->getOutputSlots(_id, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//const Identifier System::createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	//{
	//	try
	//	{
	//		return m_Engine->createSequence(_name, _idA, _idB, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//const Identifier System::createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	//{
	//	try
	//	{
	//		return m_Engine->createSynchronization(_name, _idA, _idB, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void System::link(Identifier const& _in, Identifier const& _out)
	//{
	//	try
	//	{
	//		m_Engine->link(_in, _out, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void System::linkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName)
	{
		m_Engine.linkSlots(outService, outName, inService, inName, m_Id);
	}

	void System::registerToException(ExceptionCallback callback)
	{
		m_Engine.registerToException(callback, m_Id);
	}

	void System::unregisterFromException(ExceptionCallback callback)
	{
		m_Engine.unregisterFromException(callback, m_Id);
	}

	void System::registerToException(ExceptionListener &listener)
	{
		m_Engine.registerToException(listener, m_Id);
	}

	void System::unregisterFromException(ExceptionListener &listener)
	{
		m_Engine.unregisterFromException(listener, m_Id);
	}

	void System::registerToNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_Engine.registerToNewData(service, name, callback, m_Id);
	}

	void System::unregisterFromNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_Engine.unregisterFromNewData(service, name, callback, m_Id);
	}

	void System::registerToNewData(Identifier const& service, std::string const& name, OutputListener &listener)
	{
		m_Engine.registerToNewData(service, name, listener, m_Id);
	}

	void System::unregisterFromNewData(Identifier const& service, std::string const& name, OutputListener &listener)
	{
		m_Engine.unregisterFromNewData(service, name, listener, m_Id);
	}

	//DataHandle System::getDataHandle(Identifier const& _service, std::string const& _out)
	//{
	//	try
	//	{
	//		return m_Engine->createDataHandle(_service, _out, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//Identifiers System::getChildren(Identifier const& _id)
	//{
	//	try
	//	{
	//		return m_Engine->getChildren(_id, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void System::start(Identifier const& id)
	{
		m_Engine.start(id, m_Id);
	}

	void System::startAll()
	{
		m_Engine.startAll(m_Id);
	}

	void System::stop(Identifier const& id)
	{
		m_Engine.stop(id, m_Id);
	}

	void System::stopAll()
	{
		m_Engine.stopAll(m_Id);
	}

	//void System::destroy(Identifier const& _id)
	//{
	//	try
	//	{
	//		m_Engine->destroy(_id, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void System::insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src)
	//{
	//	try
	//	{
	//		m_Engine->insert(_dst, _index, _src, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void System::append(Identifier const& _dst, Identifier const& _src)
	//{
	//	try
	//	{
	//		m_Engine->append(_dst, _src, *m_Id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}
}