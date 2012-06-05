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
		m_Impl(new SystemImpl(name))
	{
	}

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

	const BlockIdentifier System::createBlock( BundleIdentifier const& bundle, std::string const& blockName, UpdatePolicy const& triggers )
	{
		return m_Impl->createServiceBlock( bundle, blockName, triggers );
	}

	//const Identifier System::createSynchronization(std::list< Identifier > const& blockIds)
	//{
	//	return m_Impl->createSyncBlock(blockIds);
	//}

	void System::setup( BlockIdentifier const& block )
	{
		m_Impl->setUp( block );
	}

	void System::setValueInternal( BlockIdentifier const& id, std::string const& param, EngineData const& value )
	{
		m_Impl->setValue( id, param, value );
	}

	void System::insertValueInternal( BlockIdentifier const& id, std::string const& param, EngineData const& value )
	{
		m_Impl->insertValue( id, param, value );
	}

	const EngineData System::getValueInternal( BlockIdentifier const& id, std::string const& name ) const
	{
		return m_Impl->getValue( id, name );
	}

	void System::link( BlockIdentifier const& outService, std::string const& outName, BlockIdentifier const& inService, std::string const& inName )
	{
		m_Impl->link( inService, inName, outService, outName );
	}

	//void System::unlinkSlots(Identifier const& outService, std::string const& outName, Identifier const& inService, std::string const& inName)
	//{
	//	//m_EngineImpl.unlinkSlots(inService, inName, outService, outName, m_Id);
	//}

	//void System::clearOutputListeners(Identifier const& outService, std::string const& outName, const bool clearCallbacks)
	//{
	//	m_EngineImpl.clearOutputListeners(outService, outName, clearCallbacks, m_Id);
	//}

	//void System::clearInputProviders(Identifier const& inService, std::string const& inName)
	//{
	//	m_EngineImpl.clearInputProviders(inService, inName, m_Id);
	//}

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

	void System::registerToNewData( BlockIdentifier const& service, std::string const& outletName, DataCallback callback, void *userData )
	{
		m_Impl->registerToNewData( service, outletName, callback, userData );
	}

	void System::unregisterFromNewData( BlockIdentifier const& service, std::string const& outletName, DataCallback callback, void *userData )
	{
		m_Impl->unregisterFromNewData( service, outletName, callback, userData );
	}

	void System::registerToNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractDataCallbackHandler &handler )
	{
		m_Impl->registerToNewData( service, outletName, handler );
	}

	void System::unregisterFromNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractDataCallbackHandler &handler )
	{
		m_Impl->unregisterFromNewData( service, outletName, handler );
	}

}