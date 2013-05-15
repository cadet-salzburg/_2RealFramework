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

#include "engine/_2RealFunctionBlockIOManager.h"
#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealInletBuffer.h"
#include "app/_2RealAppData.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "helpers/_2RealStringHelpers.h"

#include <sstream>

using std::list;
using std::string;
using std::ostringstream;

namespace _2Real
{

	FunctionBlockIOManager::FunctionBlockIOManager( AbstractUberBlock &owner ) :
		Handleable< FunctionBlockIOManager, bundle::BlockHandle >( *this ),
		AbstractIOManager( owner )
	{
	}

	FunctionBlockIOManager::~FunctionBlockIOManager()
	{
		for ( InletIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			delete *it;
		}

		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			delete *it;
		}
	}

	AppInletHandles const& FunctionBlockIOManager::getAppInletHandles() const
	{
		return m_AppInletHandles;
	}

	AppOutletHandles const& FunctionBlockIOManager::getAppOutletHandles() const
	{
		return m_AppOutletHandles;
	}

	BundleInletHandles const& FunctionBlockIOManager::getBundleInletHandles() const
	{
		return m_BundleInletHandles;
	}

	BundleOutletHandles const& FunctionBlockIOManager::getBundleOutletHandles() const
	{
		return m_BundleOutletHandles;
	}

	void FunctionBlockIOManager::registerToNewData( AbstractCallback< list< app::AppData > const& > &cb )
	{
		m_AppEvent.addListener( cb );
	}

	void FunctionBlockIOManager::unregisterFromNewData( AbstractCallback< list< app::AppData > const& > &cb )
	{
		m_AppEvent.removeListener( cb );
	}

	app::InletHandle & FunctionBlockIOManager::getAppInletHandle( string const& name )
	{
		return getInletIO( name ).getHandle();
	}

	app::OutletHandle & FunctionBlockIOManager::getAppOutletHandle( string const& name )
	{
		return getOutletIO( name ).getHandle();
	}

	bundle::InletHandle & FunctionBlockIOManager::getBundleInletHandle( string const& name ) const
	{
		return getInletIO( name ).getBundleInletHandle();
	}

	bundle::OutletHandle & FunctionBlockIOManager::getBundleOutletHandle( string const& name ) const
	{
		return getOutletIO( name ).m_Outlet->getHandle();
	}

	AbstractInletIO & FunctionBlockIOManager::getInletIO( string const& name )
	{
		for ( InletIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( toLower( ( *it )->info().baseName ) == toLower( name ) )
			{
				return **it;
			}
		}

		ostringstream msg;
		msg << "inlet " << name<< " not found in" << m_Owner.getFullName();
		throw NotFoundException( msg.str() );
	}

	OutletIO & FunctionBlockIOManager::getOutletIO( string const& name )
	{
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( toLower( ( *it )->m_Outlet->getName() ) == toLower( name ) )
			{
				return **it;
			}
		}

		ostringstream msg;
		msg << "outlet " << name<< " not found in" << m_Owner.getFullName();
		throw NotFoundException( msg.str() );
	}

	AbstractInletIO const& FunctionBlockIOManager::getInletIO( string const& name ) const
	{
		for ( InletConstIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( toLower( ( *it )->info().baseName ) == toLower( name ) )
			{
				return **it;
			}
		}

		ostringstream msg;
		msg << "inlet " << name<< " not found in" << m_Owner.getFullName();
		throw NotFoundException( msg.str() );
	}

	OutletIO const& FunctionBlockIOManager::getOutletIO( string const& name ) const
	{
		for ( OutletConstIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( toLower( ( *it )->m_Outlet->getName() ) == toLower( name ) )
			{
				return **it;
			}
		}

		ostringstream msg;
		msg << "outlet " << name<< " not found in" << m_Owner.getFullName();
		throw NotFoundException( msg.str() );
	}

	void FunctionBlockIOManager::addBasicInlet( AbstractInletIO::InletInfo const& info )
	{
		BasicInletIO *io = new BasicInletIO( m_Owner, *m_UpdatePolicy, info );
		io->syncInletData();
		m_UpdatePolicy->addInlet( *io, info.policy );
		m_Inlets.push_back( io );
		m_AppInletHandles.push_back( io->getHandle() );
		m_BundleInletHandles.push_back( io->getBundleInletHandle() );
	}

	void FunctionBlockIOManager::addMultiInlet( AbstractInletIO::InletInfo const& info )
	{
		MultiInletIO *io = new MultiInletIO( m_Owner, *m_UpdatePolicy, info );
		m_Inlets.push_back( io );
		m_AppInletHandles.push_back( io->getHandle() );
		m_BundleInletHandles.push_back( io->getBundleInletHandle() );
	}

	void FunctionBlockIOManager::addOutlet( std::string const& name, TypeDescriptor const& type, Any const& initialValue )
	{
		OutletIO *io = new OutletIO( m_Owner, name, type, initialValue );
		io->m_Outlet->synchronize();
		m_Outlets.push_back( io );
		m_AppOutletHandles.push_back( io->getHandle() );
		m_BundleOutletHandles.push_back( io->m_Outlet->getHandle() );
	}

	void FunctionBlockIOManager::updateInletData()
	{
		for ( InletIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			( *it )->syncInletChanges();		// makes changes to multiinlet visible

			for ( unsigned int i = 0; i<( *it )->getSize(); ++i )
			{
				BasicInletIO &inletIO = ( **it )[ i ];
				inletIO.syncInletData();		// makes changed data visible
			}
		}
	}

	void FunctionBlockIOManager::updateOutletData()
	{
		list< app::AppData > data;
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			Outlet &outlet = *( ( *it )->m_Outlet );
			bool wasDiscarded = outlet.Outlet::synchronize();

			if ( !wasDiscarded )
			{
				TimestampedData lastData = outlet.getData();
				( *it )->m_InletEvent->notify( lastData );
				app::AppData out = app::AppData( lastData, outlet.getTypename(), outlet.getLongTypename(), outlet.getName() );
				( *it )->m_AppEvent->notify( out );
				data.push_back( out );
			}
		}

		if ( data.size() > 0 )
		{
			m_AppEvent.notify( data );
		}
	}

	void FunctionBlockIOManager::clearInletBuffers()
	{
		for ( InletIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			for ( unsigned int i = 0; i<( *it )->getSize(); ++i )
			{
				BasicInletIO &inletIO = ( **it )[ i ];
				inletIO.clearBufferedData();
			}
		}
	}

	void FunctionBlockIOManager::updateInletBuffers( const bool enableTriggering )
	{
		for ( InletIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			for ( unsigned int i = 0; i<( *it )->getSize(); ++i )
			{
				BasicInletIO &inletIO = ( **it )[ i ];
				inletIO.processBufferedData( enableTriggering );
			}
		}
	}

}