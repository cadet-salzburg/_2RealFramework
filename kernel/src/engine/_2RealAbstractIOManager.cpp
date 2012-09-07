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

#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "engine/_2RealAbstractUpdatePolicy.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealInletBuffer.h"

using std::string;

namespace _2Real
{

	AbstractInletIO::AbstractInletIO( AbstractUberBlock &owner, AbstractUpdatePolicy &policy, InletInfo const& info ) :
		NonCopyable< AbstractInletIO >(),
		Handleable< AbstractInletIO, app::InletHandle >( *this ),
		m_Info( info ),
		m_OwningBlock( owner ),
		m_Policy( policy )
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BasicInletIO::BasicInletIO( AbstractUberBlock &owner, AbstractUpdatePolicy &policy, InletInfo const& info ) :
		AbstractInletIO( owner, policy, info ),
		m_Inlet( new BasicInlet( owner, info.baseName ) ),
		m_Buffer( new BasicInletBuffer( info.initValue.anyValue, info.options ) )
	{
	}

	BasicInletIO::~BasicInletIO()
	{
	}

	bundle::InletHandle & BasicInletIO::getBundleInletHandle() const
	{
		return m_Inlet->getHandle();
	}
	
	std::string const& BasicInletIO::getName() const
	{
		return m_Info.baseName;
	}

	TimestampedData const& BasicInletIO::getData() const
	{
		return m_Inlet->getCurrentData();
	}

	void BasicInletIO::setBufferSize( const unsigned int size )
	{
		m_Buffer->setBufferSize( size );
	}

	void BasicInletIO::updateWhenInletDataNew( const bool isSingleWeight )
	{
		std::string policyAsString = ( isSingleWeight ? "or_newer_data" : "and_newer_data" );

		m_Policy.setNewInletPolicy( *this, new InletTriggerCtor< NewerTimestamp >, isSingleWeight, policyAsString );
	}

	void BasicInletIO::updateWhenInletDataValid()
	{
		m_Policy.setNewInletPolicy( *this, new InletTriggerCtor< ValidData >, false, "valid_data" );
	}

	void BasicInletIO::receiveData( Any const& dataAsAny )
	{
		m_Buffer->receiveData( dataAsAny );
	}

	void BasicInletIO::receiveData( std::string const& dataAsString )
	{
		m_Buffer->receiveData( dataAsString );
	}

	void BasicInletIO::setInitialValue( Any const& any )
	{
		m_Buffer->setInitialValue( any );
	}

	void BasicInletIO::syncInletData()
	{
		m_Inlet->setData( m_Buffer->getTriggeringData() );
	}

	void BasicInletIO::processBufferedData( const bool enableTriggering )
	{
		m_Buffer->processBufferedData( enableTriggering );
	}

	void BasicInletIO::clearBufferedData()
	{
		m_Buffer->clearBufferedData();
		m_Buffer->receiveData( m_Buffer->getInitialValue() );
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MultiInletIO::MultiInletIO( AbstractUberBlock &owner, AbstractUpdatePolicy &policy, InletInfo const& info ) :
		AbstractInletIO( owner, policy, info ),
		m_Inlet( new MultiInlet( owner, info.baseName ) ),
		m_Buffer( new MultiInletBuffer( info.initValue.anyValue, info.options ) )
	{
		// adding the very first inlet
		addBasicInlet();
	}

	MultiInletIO::~MultiInletIO()
	{
	}

	BasicInletIO &  MultiInletIO::operator[]( const unsigned int index )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		try
		{
			return *( m_InletIOs.at( index ).io );
		}
		catch ( std::out_of_range &e )
		{
			throw Exception( e.what() );
		}
	}

	bundle::InletHandle & MultiInletIO::getBundleInletHandle() const
	{
		return m_Inlet->getHandle();
	}

	AbstractInletIO * MultiInletIO::addBasicInlet()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		// basic inlets still need unique names
		// however, they do not necessarily correspond to the order within the vector
		unsigned int size = m_InletIOs.size();
		std::ostringstream name;
		name << m_Info.baseName << ":" << size;
		// basic inlet is created & stored, but not yed added to the multiinlet - the owning block might be in update / setup
		InletInfo info( m_Info );
		info.baseName = name.str();
		BasicInletIO *io = new BasicInletIO( m_OwningBlock, m_Policy, info );
		m_InletIOs.push_back( IO( io ) );
		// causes inlet to be added to the policy
		m_Policy.addInlet( *io );
		io->receiveData( m_Info.initValue.anyValue );

		return io;
	}

	void MultiInletIO::removeBasicInlet( AbstractInletIO *io )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		for ( BasicIOIterator it = m_InletIOs.begin(); it != m_InletIOs.end(); ++it )
		{
			if ( io == ( *it ).io )
			{
				if ( m_InletIOs.size() == 1 )	throw IllegalActionException( "cannot remove last inlet from multiinlet" );

				// kill all links involving this inlet
				// m_Engine

				// delete the buffer

				// once the app programmer calls 'remove', the inlet should not influence the update policy any more, so remove it
				m_Policy.removeInlet( *( *it ).io );

				// basic inlet must stay in the multiinlet, since owning block might be in update /setup & thus the inlet might be accessed
				( *it ).todoRemove = true;

				// done - basic inlets are unique within the multiinlet
				return;
			}
		}

		// ( ? ) not found exc if abstr inlet does not belong to multiinlet?
	}

	void MultiInletIO::syncInletChanges()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		for ( BasicIOIterator it = m_InletIOs.begin(); it != m_InletIOs.end(); ++it )
		{
			if ( ( *it ).todoAdd &! ( *it ).todoRemove )
			{
				BasicInletIO *io = ( *it ).io;
				m_Inlet->addBasicInlet( io->getInlet() );
				m_Buffer->addBasicBuffer( io->getBuffer() );

				( *it ).todoAdd = false;
			}
			else if ( ( *it ).todoAdd && ( *it ).todoRemove ) {}		// added & removed again in same cycle
			else if ( ( *it ).todoRemove )
			{
				BasicInletIO *io = ( *it ).io;
				m_Inlet->removeBasicInlet( io->getInlet() );
				m_Buffer->removeBasicBuffer( io->getBuffer() );

				it = m_InletIOs.erase( it );
			}
			else {}														// nothing
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	OutletIO::OutletIO( AbstractUberBlock &owner, std::string const& name, TypeDescriptor const& type, Any const& initialValue ) :
		Handleable< OutletIO, app::OutletHandle >( *this ),
		m_Outlet( new Outlet( owner, name, type, initialValue ) ),
		m_AppEvent( new CallbackEvent< app::AppData const& >() ),
		m_InletEvent( new CallbackEvent< TimestampedData const& >() )
	{
	}

	OutletIO::~OutletIO()
	{
		delete m_Outlet;
		delete m_AppEvent;
		delete m_InletEvent;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	AbstractIOManager::AbstractIOManager( AbstractUberBlock &owner ) :
		m_Owner( owner )
	{
	}

	AbstractIOManager::~AbstractIOManager()
	{
	}

	const string AbstractIOManager::getName() const
	{
		return m_Owner.getFullName();
	}

}