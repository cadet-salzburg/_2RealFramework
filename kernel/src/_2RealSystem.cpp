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
#include "_2RealSystemBlock.h"
#include "_2RealEngineImpl.h"
#include <iostream>

namespace _2Real
{
	System::System(std::string const& name) :
		m_Impl( new SystemBlock( EngineImpl::instance().createBlockId( name ) ) )
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

	const BlockIdentifier System::createBlock( BundleIdentifier const& bundle, std::string const& blockName )
	{
		return m_Impl->createFunctionBlock( bundle, blockName );
	}

	UpdatePolicyHandle System::getUpdatePolicy( BlockIdentifier const& block )
	{
		return m_Impl->getUpdatePolicy( block );
	}

	void System::setup( BlockIdentifier const& block )
	{
		m_Impl->setUp( block );
	}

	void System::start( BlockIdentifier const& block )
	{
		m_Impl->start( block );
	}

	void System::stop( BlockIdentifier const& block, const long timeout )
	{
		m_Impl->stop( block, timeout );
	}

	void System::singleStep( BlockIdentifier const& block )
	{
		m_Impl->singleStep( block );
	}

	void System::destroy( BlockIdentifier const& block )
	{
		m_Impl->destroy( block );
	}

	void System::setValueInternal( BlockIdentifier const& id, std::string const& param, EngineData const& value )
	{
		m_Impl->setValue( id, param, value );
	}

	EngineData const& System::getValueInternal( BlockIdentifier const& id, std::string const& name ) const
	{
		return m_Impl->getValue( id, name );
	}

	void System::link( BlockIdentifier const& outService, std::string const& outName, BlockIdentifier const& inService, std::string const& inName )
	{
		m_Impl->link( inService, inName, outService, outName );
	}

	void System::registerToException( ExceptionCallback callback, void *userData )
	{
		m_Impl->registerToException( callback, userData );
	}

	void System::unregisterFromException( ExceptionCallback callback, void *userData )
	{
		m_Impl->unregisterFromException( callback, userData );
	}

	void System::registerToExceptionInternal( AbstractExceptionCallbackHandler &handler )
	{
		m_Impl->registerToException( handler );
	}

	void System::unregisterFromExceptionInternal( AbstractExceptionCallbackHandler &handler )
	{
		m_Impl->unregisterFromException( handler );
	}

	void System::registerToOutletData( BlockIdentifier const& service, std::string const& outletName, OutletCallback callback, void *userData )
	{
		m_Impl->registerToNewData( service, outletName, callback, userData );
	}

	void System::unregisterFromOutletData( BlockIdentifier const& service, std::string const& outletName, OutletCallback callback, void *userData )
	{
		m_Impl->unregisterFromNewData( service, outletName, callback, userData );
	}

	void System::registerToAllOutletData( BlockIdentifier const& service, OutputCallback callback, void *userData )
	{
		m_Impl->registerToNewData( service, callback, userData );
	}

	void System::unregisterFromAllOutletData( BlockIdentifier const& service, OutputCallback callback, void *userData )
	{
		m_Impl->unregisterFromNewData( service, callback, userData );
	}

	void System::registerToNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractOutletCallbackHandler &handler )
	{
		m_Impl->registerToNewData( service, outletName, handler );
	}

	void System::unregisterFromNewDataInternal( BlockIdentifier const& service, std::string const& outletName, AbstractOutletCallbackHandler &handler )
	{
		m_Impl->unregisterFromNewData( service, outletName, handler );
	}

	void System::registerToNewDataInternal( BlockIdentifier const& service, AbstractOutputCallbackHandler &handler )
	{
		m_Impl->registerToNewData( service, handler );
	}

	void System::unregisterFromNewDataInternal( BlockIdentifier const& service, AbstractOutputCallbackHandler &handler )
	{
		m_Impl->unregisterFromNewData( service, handler );
	}

}