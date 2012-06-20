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

#include "_2RealServiceBlockIO.h"
#include "_2RealException.h"
#include "_2RealAbstractUberBlock.h"
#include "_2RealSetupParameter.h"
#include "_2RealInlet.h"
#include "_2RealOutlet.h"
#include "_2RealInlet.h"
#include "_2RealBlockData.h"
#include "_2RealParameterData.h"
#include "_2RealUpdatePolicyImpl.h"

#include <sstream>

using std::map;
using std::string;
using std::make_pair;

namespace _2Real
{

	ServiceIO::ServiceIO(AbstractBlock &owner) :
		AbstractIOManager(owner)
	{
	}

	ServiceIO::~ServiceIO()
	{
		clear();

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

	void ServiceIO::initFrom( BlockData const& meta )
	{
		std::map< std::string, ParameterData > const& setup = meta.getParameters();
		std::map< std::string, ParameterData > const& input = meta.getInlets();
		std::map< std::string, ParameterData > const& output = meta.getOutlets();

		for ( std::map< std::string, ParameterData >::const_iterator it = setup.begin(); it != setup.end(); ++it )
		{
			ParameterData const& meta = it->second;
			SetupParameter &param = createSetupParameterFromParameterData( meta );
			m_Params.insert( make_pair( param.getName(), &param ) );
		}

		for ( std::map< std::string, ParameterData >::const_iterator it = input.begin(); it != input.end(); ++it )
		{
			ParameterData const& meta = it->second;
			Inlet& inlet = createInletFromParameterData( meta );
			m_Inlets.insert( make_pair( inlet.getName(), &inlet ) );
		}

		for (std::map< std::string, ParameterData >::const_iterator it = output.begin(); it != output.end(); ++it )
		{
			ParameterData const& meta = it->second;
			Outlet& outlet = createOutletFromParameterData( meta );
			m_Outlets.insert( make_pair( outlet.getName(), &outlet ) );
		}
	}

	void ServiceIO::clear()
	{
		for (InletMap::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			it->second->clearLinks();
		}

		for (OutletMap::iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			it->second->clearLinks();
		}

		for (InletMap::iterator it = m_Inlets.begin(); it != m_Inlets.end(); /**/)
		{
			delete it->second;
			it = m_Inlets.erase(it);
		}

		for (OutletMap::iterator it = m_Outlets.begin(); it != m_Outlets.end(); /**/)
		{
			it->second->clearLinks();
			delete it->second;
			it = m_Outlets.erase(it);
		}

		for (ParamMap::iterator it = m_Params.begin(); it != m_Params.end(); /**/)
		{
			delete it->second;
			it = m_Params.erase(it);
		}
	}

	InletMap const& ServiceIO::getInlets() const
	{
		return m_Inlets;
	}

	OutletMap const& ServiceIO::getOutlets() const
	{
		return m_Outlets;
	}

	void ServiceIO::registerToNewData( std::string const& outName, OutletCallback callback, void *userData )
	{
		OutletFunctionCallback *cb = new OutletFunctionCallback( callback, userData );
		
		OutletFunctionCallbacks::iterator it = m_OutletFunctionCallbacks.find( cb );
		if ( it == m_OutletFunctionCallbacks.end() )
		{
			m_OutletFunctionCallbacks.insert( cb );
			
			OutletMap::const_iterator outletIt = m_Outlets.find(outName);
			if (outletIt != m_Outlets.end())
			{
				outletIt->second->registerCallback( *cb );
			}
		}
		else
		{
			delete cb;
		}
	}

	void ServiceIO::unregisterFromNewData( std::string const& outName, OutletCallback callback, void *userData )
	{
		OutletFunctionCallback *cb = new OutletFunctionCallback( callback, userData );

		OutletFunctionCallbacks::iterator it = m_OutletFunctionCallbacks.find( cb );
		if ( it != m_OutletFunctionCallbacks.end() )
		{
			OutletMap::const_iterator outletIt = m_Outlets.find(outName);
			if (outletIt != m_Outlets.end())
			{
				outletIt->second->unregisterCallback( **it );
			}

			delete *it;
			m_OutletFunctionCallbacks.erase(it);
		}

		delete cb;
	}

	void ServiceIO::registerToNewData( std::string const& outName, AbstractOutletCallbackHandler &handler )
	{
		OutletCallbackHandlers::iterator it = m_OutletCallbackHandlers.find( &handler );
		if ( it == m_OutletCallbackHandlers.end() )
		{
			m_OutletCallbackHandlers.insert( &handler );

			OutletMap::const_iterator outletIt = m_Outlets.find(outName);
			if (outletIt != m_Outlets.end())
			{
				outletIt->second->registerCallback( handler );
			}
		}
		else
		{
			delete &handler;
		}
	}

	void ServiceIO::unregisterFromNewData( std::string const& outName, AbstractOutletCallbackHandler &handler )
	{
		OutletCallbackHandlers::iterator it = m_OutletCallbackHandlers.find( &handler );
		if ( it != m_OutletCallbackHandlers.end() )
		{
			OutletMap::const_iterator outletIt = m_Outlets.find(outName);
			if (outletIt != m_Outlets.end())
			{
				outletIt->second->unregisterCallback( **it );
			}

			delete *it;
			m_OutletCallbackHandlers.erase(it);
		}

		delete &handler;
	}

	void ServiceIO::registerToNewData( OutputCallback callback, void *userData )
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

	void ServiceIO::unregisterFromNewData( OutputCallback callback, void *userData )
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

	void ServiceIO::registerToNewData( AbstractOutputCallbackHandler &handler )
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

	void ServiceIO::unregisterFromNewData( AbstractOutputCallbackHandler &handler )
	{
		OutputCallbackHandlers::iterator it = m_OutputCallbackHandlers.find( &handler );
		if ( it != m_OutputCallbackHandlers.end() )
		{
			delete *it;
			m_OutputCallbackHandlers.erase(it);
		}

		delete &handler;
	}

	const EngineData ServiceIO::getValue(std::string const& name) const
	{
		ParamMap::const_iterator paramIt = m_Params.find(name);
		if (paramIt != m_Params.end())
		{
			return paramIt->second->getValue();
		}

		InletMap::const_iterator inletIt = m_Inlets.find(name);
		if (inletIt != m_Inlets.end())
		{
			return inletIt->second->getCurrentData();
		}

		OutletMap::const_iterator outletIt = m_Outlets.find(name);
		if (outletIt != m_Outlets.end())
		{
			return outletIt->second->getLastData();
		}

		std::ostringstream msg;
		msg << "no inlet / outlet / setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	std::string const& ServiceIO::getKey(std::string const& name) const
	{
		ParamMap::const_iterator paramIt = m_Params.find(name);
		if (paramIt != m_Params.end())
		{
			return paramIt->second->getTypename();
		}

		InletMap::const_iterator inletIt = m_Inlets.find(name);
		if (inletIt != m_Inlets.end())
		{
			return inletIt->second->getTypename();
		}

		std::ostringstream msg;
		msg << "no inlet or setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	void ServiceIO::setValue(std::string const& name, TimestampedData const& value)
	{
		ParamMap::iterator paramIt = m_Params.find(name);
		if (paramIt != m_Params.end())
		{
			paramIt->second->setValue( value.getData() );
			return;
		}

		InletMap::iterator inletIt = m_Inlets.find( name );
		if (inletIt != m_Inlets.end())
		{
			inletIt->second->setFixedData( value );
			return;
		}

		std::ostringstream msg;
		msg << "no inlet or setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	void ServiceIO::insertValue(std::string const& name, TimestampedData &value)
	{
		InletMap::iterator inletIt = m_Inlets.find(name);
		if (inletIt != m_Inlets.end())
		{
			inletIt->second->receiveData( value );
		}

		std::ostringstream msg;
		msg << "no inlet named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	InletHandle ServiceIO::createInletHandle(std::string const& name)
	{
		InletMap::iterator it = m_Inlets.find(name);
		if (it != m_Inlets.end())
		{
			return InletHandle(*(it->second));
		}

		std::ostringstream msg;
		msg << "no inlet named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	OutletHandle ServiceIO::createOutletHandle(std::string const& name)
	{
		OutletMap::iterator it = m_Outlets.find(name);
		if (it != m_Outlets.end())
		{
			return  OutletHandle(*(it->second));
		}

		std::ostringstream msg;
		msg << "no outlet named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	void ServiceIO::linkWith(std::string const& inlet, AbstractBlock &out, std::string const& outlet)
	{
		InletMap::iterator itIn = m_Inlets.find(inlet);
		if (itIn == m_Inlets.end())
		{
			std::ostringstream msg;
			msg << "no inlet named " << inlet << " found in " << m_Owner.getName();
			throw NotFoundException(msg.str());
		}

		AbstractIOManager &mgr = out.getIOManager();
		OutletMap const& outlets = mgr.getOutlets();			//might throw!
		OutletMap::const_iterator itOut = outlets.find(outlet);
		if (itOut == outlets.end())
		{
			std::ostringstream msg;
			msg << "no outlet named " << outlet << " found in " << out.getName();
			throw NotFoundException(msg.str());
		}

		itIn->second->linkWith(*(itOut->second));
	}

	void ServiceIO::syncInlets()
	{
		for (InletMap::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it)
		{
			it->second->updateCurrentData();
		}
	}

	void ServiceIO::updateOutlets()
	{
		std::list< OutputData > data;
		for (OutletMap::iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it)
		{
			it->second->update();
			data.push_back( it->second->getLastOutputData() );
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

	void ServiceIO::updateInlets()
	{
		for (InletMap::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it)
		{
			it->second->resetData();
		}
	}

	void ServiceIO::subBlockAdded(AbstractBlock &subBlock)
	{
	}

	void ServiceIO::subBlockRemoved(AbstractBlock &subBlock)
	{
	}

	void ServiceIO::uberBlockAdded(AbstractBlock &uberBlock)
	{
	}

	void ServiceIO::uberBlockRemoved(AbstractBlock &uberBlock)
	{
	}
}