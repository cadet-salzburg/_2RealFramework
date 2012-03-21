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

#include "_2RealSystem.h"
#include "_2RealSystemImpl.h"

namespace _2Real
{
	System::System(std::string const& name) :
		m_Impl(new SystemImpl())
	{
	}

	//System::System(std::string const& name, std::string const& xmlFile) :
	//	m_EngineImpl(EngineImpl::instance())
	//{
	//}

	System::System(System const& src) :
		m_Impl(src.m_Impl)
	{
	}

	System::~System()
	{
		delete m_Impl;
	}

	void System::clear()
	{
		m_Impl->clear();
	}

	const Identifier System::createService(std::string const& name, Identifier const& plugin, std::string const& service, UpdateTriggers const& triggers)
	{
		//std::string idName = validateName(name);
		//unique(idName);

		return m_Impl->createService(name, plugin, service, triggers);
		//m_Lookup.insert(NamedId(idName, id));
	}

	const Identifier System::createService(Identifier const& plugin, std::string const& service, UpdateTriggers const& triggers)
	{
		return m_Impl->createService(plugin, service, triggers);
		//m_Lookup.insert(NamedId(id.name(), id));
	}

	void System::setup(Identifier const& runnableId)
	{
		m_Impl->setUp(runnableId);
	}

	void System::setValueInternal(Identifier const& id, std::string const& param, EngineData const& value)
	{
		m_Impl->setValue(id, param, value);
	}

	const EngineData System::getValueInternal(Identifier const& id, std::string const& name) const
	{
		return m_Impl->getValue(id, name);
	}

	void System::linkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName)
	{
		m_Impl->linkSlots(inService, inName, outService, outName);
	}

	void System::unlinkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName)
	{
		//m_EngineImpl.unlinkSlots(inService, inName, outService, outName, m_Id);
	}

	void System::clearOutputListeners(Identifier const& outService, std::string const& outName, const bool clearCallbacks)
	{
		//m_EngineImpl.clearOutputListeners(outService, outName, clearCallbacks, m_Id);
	}

	void System::clearInputProviders(Identifier const& inService, std::string const& inName)
	{
		//m_EngineImpl.clearInputProviders(inService, inName, m_Id);
	}

	//void System::registerToException(ExceptionCallback callback)
	//{
	//}

	//void System::unregisterFromException(ExceptionCallback callback)
	//{
	//}

	//void System::registerToException(IExceptionListener &listener)
	//{
	//}

	//void System::unregisterFromException(IExceptionListener &listener)
	//{
	//}

	//void System::registerToStateChange(Identifier const& runnableId, StateChangeCallback callback)
	//{
	//}

	//void System::unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback)
	//{
	//}

	//void System::registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	//{
	//}

	//void System::unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	//{
	//}

	void System::registerToNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_Impl->registerToNewData(service, name, callback);
	}

	void System::unregisterFromNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_Impl->unregisterFromNewData(service, name, callback);
	}

	//void System::registerToNewData(Identifier const& service, std::string const& name, IOutputListener &listener)
	//{
	//}

	//void System::unregisterFromNewData(Identifier const& service, std::string const& name, IOutputListener &listener)
	//{
	//}

	void System::unique(std::string const& s) const
	{
		if (m_Lookup.find(s) != m_Lookup.end())
		{
			std::ostringstream msg;
			msg << "name " << s << " already exists!";
			throw AlreadyExistsException(msg.str());
		}
	}
}