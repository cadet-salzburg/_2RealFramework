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
#include "_2RealSetupParameter.h"
#include "_2RealInlet.h"
#include "_2RealOutlet.h"
#include "_2RealInletBuffer.h"
#include "_2RealParameterData.h"
#include "_2RealLink.h"
#include "_2RealHelpersInternal.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"


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

		for ( OutletFunctionCallbacks::iterator it = m_OutletFunctionCallbacks.begin(); it != m_OutletFunctionCallbacks.end(); ++it )
		{
			delete *it;
		}

		for ( OutletCallbackHandlers::iterator it = m_OutletCallbackHandlers.begin(); it != m_OutletCallbackHandlers.end(); ++it )
		{
			delete *it;
		}

		for ( OutputFunctionCallbacks::iterator it = m_OutputFunctionCallbacks.begin(); it != m_OutputFunctionCallbacks.end(); ++it )
		{
			delete *it;
		}

		for ( OutputCallbackHandlers::iterator it = m_OutputCallbackHandlers.begin(); it != m_OutputCallbackHandlers.end(); ++it )
		{
			delete *it;
		}
	}

	void FunctionBlockIOManager::registerToNewData( std::string const& outName, OutletCallback callback, void *userData )
	{
		OutletFunctionCallback *cb = new OutletFunctionCallback( callback, userData );
		
		OutletFunctionCallbacks::iterator it = m_OutletFunctionCallbacks.find( cb );
		if ( it == m_OutletFunctionCallbacks.end() )
		{
			m_OutletFunctionCallbacks.insert( cb );
		}
		else
		{
			delete cb;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( std::string const& outName, OutletCallback callback, void *userData )
	{
		OutletFunctionCallback *cb = new OutletFunctionCallback( callback, userData );

		OutletFunctionCallbacks::iterator it = m_OutletFunctionCallbacks.find( cb );
		if ( it != m_OutletFunctionCallbacks.end() )
		{
			delete *it;
			m_OutletFunctionCallbacks.erase(it);
		}

		delete cb;
	}

	void FunctionBlockIOManager::registerToNewData( std::string const& outName, AbstractOutletCallbackHandler &handler )
	{
		OutletCallbackHandlers::iterator it = m_OutletCallbackHandlers.find( &handler );
		if ( it == m_OutletCallbackHandlers.end() )
		{
			m_OutletCallbackHandlers.insert( &handler );
		}
		else
		{
			delete &handler;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( std::string const& outName, AbstractOutletCallbackHandler &handler )
	{
		OutletCallbackHandlers::iterator it = m_OutletCallbackHandlers.find( &handler );
		if ( it != m_OutletCallbackHandlers.end() )
		{
			delete *it;
			m_OutletCallbackHandlers.erase(it);
		}

		delete &handler;
	}

	void FunctionBlockIOManager::registerToNewData( OutputCallback callback, void *userData )
	{
		OutputFunctionCallback *cb = new OutputFunctionCallback( callback, userData );
		
		OutputFunctionCallbacks::iterator it = m_OutputFunctionCallbacks.find( cb );
		if ( it == m_OutputFunctionCallbacks.end() )
		{
			m_OutputFunctionCallbacks.insert( cb );
		}
		else
		{
			delete cb;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( OutputCallback callback, void *userData )
	{
		OutputFunctionCallback *cb = new OutputFunctionCallback( callback, userData );

		OutputFunctionCallbacks::iterator it = m_OutputFunctionCallbacks.find( cb );
		if ( it != m_OutputFunctionCallbacks.end() )
		{
			delete *it;
			m_OutputFunctionCallbacks.erase(it);
		}

		delete cb;
	}

	void FunctionBlockIOManager::registerToNewData( AbstractOutputCallbackHandler &handler )
	{
		OutputCallbackHandlers::iterator it = m_OutputCallbackHandlers.find( &handler );
		if ( it == m_OutputCallbackHandlers.end() )
		{
			m_OutputCallbackHandlers.insert( &handler );
		}
		else
		{
			delete &handler;
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( AbstractOutputCallbackHandler &handler )
	{
		OutputCallbackHandlers::iterator it = m_OutputCallbackHandlers.find( &handler );
		if ( it != m_OutputCallbackHandlers.end() )
		{
			delete *it;
			m_OutputCallbackHandlers.erase(it);
		}

		delete &handler;
	}

	EngineData const& FunctionBlockIOManager::getValue( string const& name ) const
	{
		SetupParameter const* const param = findSetupParameter( name );
		if ( param != nullptr )
		{
			return param->getParameterValue();
		}

		Inlet const* const inlet = findInlet( name );
		if ( inlet != nullptr )
		{
			return inlet->getCurrentValue();
		}

		Outlet const* const outlet = findOutlet( name );
		if ( outlet != nullptr )
		{
			return outlet->getLastData();
		}

		ostringstream msg;
		msg << "no inlet / outlet / setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException( msg.str() );
	}

	void FunctionBlockIOManager::setValue( string const& name, TimestampedData const& value )
	{
		SetupParameter * const param = findSetupParameter( name );
		if ( param != nullptr )
		{
			param->setParameterValue( value.getData() );
			return;
		}

		Inlet * const inlet = findInlet( name );
		if ( inlet != nullptr )
		{
			inlet->setDefaultValue( value );
			return;
		}

		ostringstream msg;
		msg << "no inlet / setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException( msg.str() );
	}

	std::string const& FunctionBlockIOManager::getTypename( string const& name ) const
	{
		SetupParameter const* const param = findSetupParameter( name );
		if ( param != nullptr )
		{
			return param->getTypename();
		}

		Inlet const* const inlet = findInlet( name );
		if ( inlet != nullptr )
		{
			return inlet->getTypename();
		}

		Outlet const* const outlet = findOutlet( name );
		if ( outlet != nullptr )
		{
			return outlet->getTypename();
		}

		ostringstream msg;
		msg << "no inlet / outlet / setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException( msg.str() );
	}

	std::string const& FunctionBlockIOManager::getLongTypename( string const& name ) const
	{
		SetupParameter const* const param = findSetupParameter( name );
		if ( param != nullptr )
		{
			return param->getLongTypename();
		}

		Inlet const* const inlet = findInlet( name );
		if ( inlet != nullptr )
		{
			return inlet->getLongTypename();
		}

		Outlet const* const outlet = findOutlet( name );
		if ( outlet != nullptr )
		{
			return outlet->getLongTypename();
		}

		ostringstream msg;
		msg << "no inlet / outlet / setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException( msg.str() );
	}

	InletHandle FunctionBlockIOManager::createInletHandle( string const& name )
	{
		return InletHandle( getInlet( name ) );
	}

	OutletHandle FunctionBlockIOManager::createOutletHandle(std::string const& name)
	{
		return OutletHandle( getOutlet( name ) );
	}

	Inlet const& FunctionBlockIOManager::getInlet( string const& name ) const
	{
		Inlet const* const inlet = findInlet( name );
		if ( inlet == nullptr )
		{
			ostringstream msg;
			msg << "inlet " << name<< " not found in" << m_Owner.getName();
			throw NotFoundException( msg.str() );
		}

		return *inlet;
	}

	Outlet const& FunctionBlockIOManager::getOutlet( string const& name ) const
	{
		Outlet const* const outlet = findOutlet( name );
		if ( outlet == nullptr )
		{
			ostringstream msg;
			msg << "outlet " << name<< " not found in" << m_Owner.getName();
			throw NotFoundException( msg.str() );
		}

		return *outlet;
	}

	SetupParameter const& FunctionBlockIOManager::getSetupParameter( string const& name ) const
	{
		SetupParameter const* const param = findSetupParameter( name );
		if ( param == nullptr )
		{
			ostringstream msg;
			msg << "parameter " << name<< " not found in" << m_Owner.getName();
			throw NotFoundException( msg.str() );
		}

		return *param;
	}

	Inlet const* const FunctionBlockIOManager::findInlet( string const& name ) const
	{
		for ( InletVector::const_iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return *it;
			}
		}

		return nullptr;
	}

	Outlet const* const FunctionBlockIOManager::findOutlet( string const& name ) const
	{
		for ( OutletVector::const_iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return *it;
			}
		}

		return nullptr;
	}

	SetupParameter const* const FunctionBlockIOManager::findSetupParameter( string const& name ) const
	{
		for ( ParamVector::const_iterator it = m_Params.begin(); it != m_Params.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return *it;
			}
		}

		return nullptr;
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

	SetupParameter & FunctionBlockIOManager::getSetupParameter( string const& name )
	{
		SetupParameter *param = findSetupParameter( name );
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

	SetupParameter * FunctionBlockIOManager::findSetupParameter( string const& name )
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
		Inlet *inlet = createInletFromParameterData( data );
		m_Inlets.push_back( inlet );
	}

	void FunctionBlockIOManager::addOutlet( ParameterData const& data )
	{
		Outlet *outlet = createOutletFromParameterData( data );
		m_Outlets.push_back( outlet );
	}

	void FunctionBlockIOManager::addSetupParameter( ParameterData const& data )
	{
		SetupParameter *param = createSetupParameterFromParameterData( data );
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
		std::list< OutputData > data;
		for ( OutletVector::iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			( *it )->update();
			OutputData out = ( *it )->getLastOutputData();

			for ( OutletFunctionCallbacks::iterator it = m_OutletFunctionCallbacks.begin(); it != m_OutletFunctionCallbacks.end(); ++it )
			{
				( *it )->invoke( out );
			}

			for ( OutletCallbackHandlers::iterator it = m_OutletCallbackHandlers.begin(); it != m_OutletCallbackHandlers.end(); ++it )
			{
				( *it )->invoke( out );
			}

			data.push_back( out );
		}

		if ( data.size() > 0 )
		{
			for ( OutputFunctionCallbacks::iterator it = m_OutputFunctionCallbacks.begin(); it != m_OutputFunctionCallbacks.end(); ++it )
			{
				( *it )->invoke( data );
			}

			for ( OutputCallbackHandlers::iterator it = m_OutputCallbackHandlers.begin(); it != m_OutputCallbackHandlers.end(); ++it )
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

}