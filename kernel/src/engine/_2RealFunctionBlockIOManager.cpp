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
#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealParameterData.h"
#include "engine/_2RealLink.h"
#include "helpers/_2RealHelpersInternal.h"
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

	FunctionBlockIOManager::FunctionBlockIOManager( FunctionBlock &owner ) :
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
			delete it->outlet;
			for ( app::OutletDataFunctionCallbacks::iterator outIt = it->callbacks.begin(); outIt != it->callbacks.end(); ++outIt )
			{
				delete *outIt;
			}

			for ( app::OutletDataCallbackHandlers::iterator outIt = it->handlers.begin(); outIt != it->handlers.end(); ++outIt )
			{
				delete *outIt;
			}
		}

		for ( ParamVector::iterator it = m_Params.begin(); it != m_Params.end(); ++it )
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

	void FunctionBlockIOManager::registerToNewData( Outlet const& outlet, app::OutletDataCallback callback, void *userData )
	{
		app::OutletDataFunctionCallback *cb = new app::OutletDataFunctionCallback( callback, userData );
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( it->outlet == &outlet )
			{
				//it->access.lock();
				app::OutletDataFunctionCallbacks::iterator cbIt = it->callbacks.find( cb );
				if ( cbIt == it->callbacks.end() )
				{
					it->callbacks.insert( cb );
				}
				else delete cb;
				//it->access.unlock();
				break;
			}
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( Outlet const& outlet, app::OutletDataCallback callback, void *userData )
	{
		app::OutletDataFunctionCallback *cb = new app::OutletDataFunctionCallback( callback, userData );
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( it->outlet == &outlet )
			{
				//it->access.lock();
				app::OutletDataFunctionCallbacks::iterator cbIt = it->callbacks.find( cb );
				if ( cbIt != it->callbacks.end() )
				{
					delete *cbIt;
					it->callbacks.erase( cbIt );
				}
				//it->access.unlock();
				delete cb;
				break;
			}
		}
	}

	void FunctionBlockIOManager::registerToNewData( Outlet const& outlet, app::AbstractOutletDataCallbackHandler &handler )
	{
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( it->outlet == &outlet )
			{
				//it->access.lock();
				app::OutletDataCallbackHandlers::iterator cbIt = it->handlers.find( &handler );
				if ( cbIt == it->handlers.end() )
				{
					it->handlers.insert( &handler );
				}
				else delete &handler;
				//it->access.unlock();
				break;
			}
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( Outlet const& outlet, app::AbstractOutletDataCallbackHandler &handler )
	{
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( it->outlet == &outlet )
			{
				//it->access.lock();
				app::OutletDataCallbackHandlers::iterator cbIt = it->handlers.find( &handler );
				if ( cbIt != it->handlers.end() )
				{
					delete *cbIt;
					it->handlers.erase( cbIt );
				}
				//it->access.unlock();
				delete &handler;
				break;
			}
		}
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
			if ( it->outlet->getName() == name )
			{
				return it->outlet;
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

	void FunctionBlockIOManager::addInlet( ParamData const& data )
	{
		Inlet *inlet = new Inlet( m_Owner, data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		m_UpdatePolicy->addInlet( *inlet );
		m_Inlets.push_back( inlet );
	}

	void FunctionBlockIOManager::addOutlet( ParamData const& data )
	{
		Outlet *outlet = new Outlet( m_Owner, data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );

		OutletIO io;
		io.outlet = outlet;

		m_Outlets.push_back( io );
	}

	void FunctionBlockIOManager::addParameter( ParamData const& data )
	{
		Parameter *parameter = new Parameter( m_Owner, data.getName(), data.getLongTypename(), data.getTypename() );
		parameter->setData( TimestampedData( data.getDefaultValue(), 0 ) );
		m_Params.push_back( parameter );
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
			it->outlet->update();

			EngineData const& lastData = it->outlet->getData();
			app::AppData out = app::AppData( lastData, it->outlet->getTypename(), it->outlet->getName() );

			// what if an outlet discarded its data?

			for ( app::OutletDataFunctionCallbacks::iterator cbIt = it->callbacks.begin(); cbIt != it->callbacks.end(); ++cbIt )
			{
				( *cbIt )->invoke( out );
			}

			for ( app::OutletDataCallbackHandlers::iterator cbIt = it->handlers.begin(); cbIt != it->handlers.end(); ++cbIt )
			{
				( *cbIt )->invoke( out );
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