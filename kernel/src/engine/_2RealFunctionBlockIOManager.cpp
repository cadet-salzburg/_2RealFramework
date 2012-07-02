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
			delete it->callbacks;
		}

		for ( ParameterVector::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it )
		{
			delete *it;
		}
	}

	void FunctionBlockIOManager::registerToNewData( Outlet const& outlet, AbstractCallback< app::AppData const& > &cb )
	{
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( it->outlet == &outlet )
			{
				it->callbacks->addListener( cb );
				break;
			}
		}
	}

	void FunctionBlockIOManager::unregisterFromNewData( Outlet const& outlet, AbstractCallback< app::AppData const& > &cb )
	{
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( it->outlet == &outlet )
			{
				it->callbacks->removeListener( cb );
				break;
			}
		}
	}

	void FunctionBlockIOManager::registerToNewData( AbstractCallback< std::list< app::AppData > const& > &cb )
	{
		m_BlockDataChanged.addListener( cb );
	}

	void FunctionBlockIOManager::unregisterFromNewData( AbstractCallback< std::list< app::AppData > const& > &cb )
	{
		m_BlockDataChanged.removeListener( cb );
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
		for ( InletIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return **it;
			}
		}

		ostringstream msg;
		msg << "inlet " << name<< " not found in" << m_Owner.getName();
		throw NotFoundException( msg.str() );
	}

	Outlet & FunctionBlockIOManager::getOutlet( string const& name )
	{
		for ( OutletIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( it->outlet->getName() == name )
			{
				return *it->outlet;
			}
		}

		ostringstream msg;
		msg << "outlet " << name<< " not found in" << m_Owner.getName();
		throw NotFoundException( msg.str() );
	}

	Parameter & FunctionBlockIOManager::getParameter( string const& name )
	{
		for ( ParameterIterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it )
		{
			if ( ( *it )->getName() == name )
			{
				return **it;
			}
		}
			
		ostringstream msg;
		msg << "parameter " << name<< " not found in" << m_Owner.getName();
		throw NotFoundException( msg.str() );
	}

	void FunctionBlockIOManager::addInlet( ParamData const& data )
	{
		Inlet *inlet = new Inlet( m_Owner, data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		m_UpdatePolicy->addInlet( *inlet );
		m_Inlets.push_back( inlet );
	}

	void FunctionBlockIOManager::addOutlet( ParamData const& data )
	{
		OutletIO io;
		io.outlet = new Outlet( m_Owner, data.getName(), data.getLongTypename(), data.getTypename(), data.getDefaultValue() );
		io.callbacks = new CallbackEvent< app::AppData const& >();
		m_Outlets.push_back( io );
	}

	void FunctionBlockIOManager::addParameter( ParamData const& data )
	{
		Parameter *parameter = new Parameter( m_Owner, data.getName(), data.getLongTypename(), data.getTypename() );
		parameter->setData( TimestampedData( data.getDefaultValue(), 0 ) );
		m_Parameters.push_back( parameter );
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

			// (?) what if an outlet discarded?
			it->callbacks->notify( out );
			data.push_back( out );
		}

		if ( data.size() > 0 )
		{
			m_BlockDataChanged.notify( data );
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
		for ( ParameterVector::iterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it )
		{
			( *it )->synchronize();
		}
	}

}