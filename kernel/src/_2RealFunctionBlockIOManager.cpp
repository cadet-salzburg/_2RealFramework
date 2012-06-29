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

#include "_2RealFunctionBlockIOManager.h"
#include "_2RealException.h"
#include "_2RealAbstractUberBlock.h"
#include "_2RealParameter.h"
#include "_2RealInlet.h"
#include "_2RealOutlet.h"
#include "_2RealInletBuffer.h"
#include "_2RealParameterData.h"
#include "_2RealLink.h"
#include "_2RealHelpersInternal.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealParameterHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "bundle/_2RealParameterHandle.h"

#include <sstream>

using std::map;
using std::string;
using std::make_pair;
using std::ostringstream;

namespace _2Real
{

	FunctionBlockIOManager::FunctionBlockIOManager( AbstractUberBlock &owner ) :
		AbstractIOManager( owner )
	{
	}

	FunctionBlockIOManager::~FunctionBlockIOManager()
	{
		for ( InletVector::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			delete *it;
		}

		for ( OutletVector::iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			delete *it;
		}

		for ( ParamVector::iterator it = m_Params.begin(); it != m_Params.end(); ++it )
		{
			delete *it;
		}

		for ( app::OutletDataFunctionCallbacks::iterator it = m_OutletDataFunctionCallbacks.begin(); it != m_OutletDataFunctionCallbacks.end(); ++it )
		{
			delete *it;
		}

		for ( app::OutletDataCallbackHandlers::iterator it = m_OutletDataCallbackHandlers.begin(); it != m_OutletDataCallbackHandlers.end(); ++it )
		{
			delete *it;
		}

		for ( app::BlockDataFunctionCallbacks::iterator it = m_BlockDataFunctionCallbacks.begin(); it != m_BlockDataFunctionCallbacks.end(); ++it )
		{
			delete *it;
		}

		for ( app::BlockDataCallbackHandlers::iterator it = m_BlockDataCallbackHandlers.begin(); it != m_BlockDataCallbackHandlers.end(); ++it )
		{
			delete *it;
		}
	}

	void FunctionBlockIOManager::registerToNewData( std::string const& outName, app::OutletDataCallback callback, void *userData )
	{
		app::OutletDataFunctionCallback *cb = new app::OutletDataFunctionCallback( callback, userData );
		app::OutletDataFunctionCallbacks::iterator it = m_OutletDataFunctionCallbacks.find( cb );
		if ( it == m_OutletDataFunctionCallbacks.end() )
		{
			m_OutletDataFunctionCallbacks.insert( cb );
		}
		else
		{
			delete cb;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( std::string const& outName, app::OutletDataCallback callback, void *userData )
	{
		app::OutletDataFunctionCallback *cb = new app::OutletDataFunctionCallback( callback, userData );
		app::OutletDataFunctionCallbacks::iterator it = m_OutletDataFunctionCallbacks.find( cb );
		if ( it != m_OutletDataFunctionCallbacks.end() )
		{
			delete *it;
			m_OutletDataFunctionCallbacks.erase(it);
		}
		delete cb;
	}

	void FunctionBlockIOManager::registerToNewData( std::string const& outName, app::AbstractOutletDataCallbackHandler &handler )
	{
		app::OutletDataCallbackHandlers::iterator it = m_OutletDataCallbackHandlers.find( &handler );
		if ( it == m_OutletDataCallbackHandlers.end() )
		{
			m_OutletDataCallbackHandlers.insert( &handler );
		}
		else
		{
			delete &handler;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( std::string const& outName, app::AbstractOutletDataCallbackHandler &handler )
	{
		app::OutletDataCallbackHandlers::iterator it = m_OutletDataCallbackHandlers.find( &handler );
		if ( it != m_OutletDataCallbackHandlers.end() )
		{
			delete *it;
			m_OutletDataCallbackHandlers.erase(it);
		}
		delete &handler;
	}

	void FunctionBlockIOManager::registerToNewData( app::BlockDataCallback callback, void *userData )
	{
		app::BlockDataFunctionCallback *cb = new app::BlockDataFunctionCallback( callback, userData );
		app::BlockDataFunctionCallbacks::iterator it = m_BlockDataFunctionCallbacks.find( cb );
		if ( it == m_BlockDataFunctionCallbacks.end() )
		{
			m_BlockDataFunctionCallbacks.insert( cb );
		}
		else
		{
			delete cb;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( app::BlockDataCallback callback, void *userData )
	{
		app::BlockDataFunctionCallback *cb = new app::BlockDataFunctionCallback( callback, userData );
		app::BlockDataFunctionCallbacks::iterator it = m_BlockDataFunctionCallbacks.find( cb );
		if ( it != m_BlockDataFunctionCallbacks.end() )
		{
			delete *it;
			m_BlockDataFunctionCallbacks.erase(it);
		}
		delete cb;
	}

	void FunctionBlockIOManager::registerToNewData( app::AbstractBlockDataCallbackHandler &handler )
	{
		app::BlockDataCallbackHandlers::iterator it = m_BlockDataCallbackHandlers.find( &handler );
		if ( it == m_BlockDataCallbackHandlers.end() )
		{
			m_BlockDataCallbackHandlers.insert( &handler );
		}
		else
		{
			delete &handler;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( app::AbstractBlockDataCallbackHandler &handler )
	{
		app::BlockDataCallbackHandlers::iterator it = m_BlockDataCallbackHandlers.find( &handler );
		if ( it != m_BlockDataCallbackHandlers.end() )
		{
			delete *it;
			m_BlockDataCallbackHandlers.erase(it);
		}

		delete &handler;
	}

	app::InletHandle FunctionBlockIOManager::createAppInletHandle( string const& name )
	{
		return app::InletHandle( getInlet( name ) );
	}

	app::OutletHandle FunctionBlockIOManager::createAppOutletHandle( string const& name )
	{
		return app::OutletHandle( getOutlet( name ) );
	}

	app::ParameterHandle FunctionBlockIOManager::createAppParameterHandle( string const& name )
	{
		return app::ParameterHandle( getParameter( name ) );
	}

	bundle::InletHandle FunctionBlockIOManager::createBundleInletHandle( string const& name )
	{
		return bundle::InletHandle( getInlet( name ) );
	}

	bundle::OutletHandle FunctionBlockIOManager::createBundleOutletHandle( string const& name )
	{
		return bundle::OutletHandle( getOutlet( name ) );
	}

	bundle::ParameterHandle FunctionBlockIOManager::createBundleParameterHandle( string const& name )
	{
		return bundle::ParameterHandle( getParameter( name ) );
	}

	Inlet & FunctionBlockIOManager::getInlet( string const& name )
	{
		Inlet *inlet = findInlet( name );
		if ( inlet == nullptr )
		{
			ostringstream msg;
			msg << "inlet " << name<< " not found in" << m_Owner.getName();
			throw NotFoundException( msg.str() );
		}

		return *inlet;
	}

	Outlet & FunctionBlockIOManager::getOutlet( string const& name )
	{
		Outlet *outlet = findOutlet( name );
		if ( outlet == nullptr )
		{
			ostringstream msg;
			msg << "outlet " << name<< " not found in" << m_Owner.getName();
			throw NotFoundException( msg.str() );
		}

		return *outlet;
	}

	Parameter & FunctionBlockIOManager::getParameter( string const& name )
	{
		Parameter *param = findParameter( name );
		if ( param == nullptr )
		{
			ostringstream msg;
			msg << "parameter " << name<< " not found in" << m_Owner.getName();
			throw NotFoundException( msg.str() );
		}

		return *param;
	}

	Inlet * FunctionBlockIOManager::findInlet( string const& name )
	{
		for ( InletVector::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return *it;
			}
		}

		return nullptr;
	}

	Outlet * FunctionBlockIOManager::findOutlet( string const& name )
	{
		for ( OutletVector::iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return *it;
			}
		}

		return nullptr;
	}

	Parameter * FunctionBlockIOManager::findParameter( string const& name )
	{
		for ( ParamVector::iterator it = m_Params.begin(); it != m_Params.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return *it;
			}
		}

		return nullptr;
	}

	void FunctionBlockIOManager::addInlet( ParameterData const& data )
	{
		Inlet *inlet = createInletFromParameterData( m_Owner, data );
		m_Inlets.push_back( inlet );
	}

	void FunctionBlockIOManager::addOutlet( ParameterData const& data )
	{
		Outlet *outlet = createOutletFromParameterData( m_Owner, data );
		m_Outlets.push_back( outlet );
	}

	void FunctionBlockIOManager::addParameter( ParameterData const& data )
	{
		Parameter *param = createParameterFromParameterData( m_Owner, data );
		m_Params.push_back( param );
	}

	void FunctionBlockIOManager::updateInletValues()
	{
		for ( InletVector::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			( *it )->updateCurrentValue();
		}
	}

	void FunctionBlockIOManager::updateOutletValues()
	{
		std::list< app::AppData > data;
		for ( OutletVector::iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			( *it )->update();

			EngineData const& lastData = ( *it )->getData();
			app::AppData out = app::AppData( lastData, (*it)->getTypename(), (*it)->getName() );

			// what if an outlet discarded its data?

			for ( app::OutletDataFunctionCallbacks::iterator it = m_OutletDataFunctionCallbacks.begin(); it != m_OutletDataFunctionCallbacks.end(); ++it )
			{
				( *it )->invoke( out );
			}

			for ( app::OutletDataCallbackHandlers::iterator it = m_OutletDataCallbackHandlers.begin(); it != m_OutletDataCallbackHandlers.end(); ++it )
			{
				( *it )->invoke( out );
			}

			data.push_back( out );
		}

		if ( data.size() > 0 )
		{
			for ( app::BlockDataFunctionCallbacks::iterator it = m_BlockDataFunctionCallbacks.begin(); it != m_BlockDataFunctionCallbacks.end(); ++it )
			{
				( *it )->invoke( data );
			}

			for ( app::BlockDataCallbackHandlers::iterator it = m_BlockDataCallbackHandlers.begin(); it != m_BlockDataCallbackHandlers.end(); ++it )
			{
				( *it )->invoke( data );
			}
		}
	}

	void FunctionBlockIOManager::updateInletBuffers()
	{
		for ( InletVector::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			( *it )->updateDataBuffer();
		}
	}

	void FunctionBlockIOManager::updateParameterValues()
	{
		for ( ParamVector::iterator it = m_Params.begin(); it != m_Params.end(); ++it )
		{
			( *it )->synchronize();
		}
	}

}