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

#include "_2RealServiceIO.h"
#include "_2RealException.h"
#include "_2RealAbstractBlock.h"
#include "_2RealSetupParameter.h"
#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealInputSlot.h"
#include "_2RealServiceMetadata.h"
#include "_2RealParameterMetadata.h"

#include <sstream>

namespace _2Real
{

	ServiceIO::ServiceIO(AbstractBlock &owner) :
		AbstractIOManager(owner),
		m_Policy(new AlwaysInsertRemoveOldest())
	{
	}

	ServiceIO::~ServiceIO()
	{
		clear();
	}

	void ServiceIO::initFrom(ServiceMetadata &meta, Poco::Timestamp const& time)
	{
		ParameterDataMap const& setup = meta.getSetupParameters();
		ParameterDataMap const& input = meta.getInputSlots();
		ParameterDataMap const& output = meta.getOutputSlots();

		for (ParameterDataMap::const_iterator it = setup.begin(); it != setup.end(); ++it)
		{
			ParameterMetadata const& meta = *it->second;
			SetupParameter *param = new SetupParameter(meta);
			m_Params.insert(std::make_pair(param->getName(), param));
		}

		for (ParameterDataMap::const_iterator it = input.begin(); it != input.end(); ++it)
		{
			ParameterMetadata const& meta = *it->second;
			InputSlot *inlet = new InputSlot(meta, *m_Policy, 50);
			m_Inlets.insert(std::make_pair(inlet->getName(), inlet));
		}

		for (ParameterDataMap::const_iterator it = output.begin(); it != output.end(); ++it)
		{
			ParameterMetadata const& meta = *it->second;
			OutputSlot *outlet = new OutputSlot(meta, time);
			m_Outlets.insert(std::make_pair(outlet->getName(), outlet));
		}
	}

	void ServiceIO::clear()
	{
		if (m_Policy != NULL)
		{
			delete m_Policy;
			m_Policy = NULL;
		}

		for (InletMap::iterator it = m_Inlets.begin(); it != m_Inlets.end(); /**/)
		{
			delete it->second;
			it = m_Inlets.erase(it);
		}

		for (OutletMap::iterator it = m_Outlets.begin(); it != m_Outlets.end(); /**/)
		{
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

	//void ServiceIO::addParam(ParameterMetadata const& meta)
	//{
	//	SetupParameter *param = new SetupParameter(meta);
	//	m_Params.insert(std::make_pair(param->getName(), param));
	//}

	//void ServiceIO::addInlet(ParameterMetadata const& meta)
	//{
	//	InputSlot *inlet = new InputSlot(meta, *m_Policy, 50);
	//	m_Inlets.insert(std::make_pair(inlet->getName(), inlet));
	//}

	//void ServiceIO::addOutlet(ParameterMetadata const& meta, Poco::Timestamp const& timestamp)
	//{
	//	OutputSlot *outlet = new OutputSlot(meta, timestamp);
	//	m_Outlets.insert(std::make_pair(outlet->getName(), outlet));
	//}

	void ServiceIO::registerToNewData(std::string const& outName, DataCallback callback)
	{
	}

	void ServiceIO::unregisterFromNewData(std::string const& outName, DataCallback callback)
	{
	}

	void ServiceIO::registerToNewData(std::string const& outName, IOutputListener &listener)
	{
	}

	void ServiceIO::unregisterFromNewData(std::string const& outName, IOutputListener &listener)
	{
	}

	const EngineData ServiceIO::getValue(std::string const& name) const
	{
		ParamMap::const_iterator paramIt = m_Params.find(name);
		if (paramIt != m_Params.end())
		{
			return paramIt->second->getData();
		}

		InletMap::const_iterator inletIt = m_Inlets.find(name);
		if (inletIt != m_Inlets.end())
		{
			return inletIt->second->getNewest();
		}

		std::ostringstream msg;
		msg << "no inlet or setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	std::string const& ServiceIO::getKey(std::string const& name) const
	{
		ParamMap::const_iterator paramIt = m_Params.find(name);
		if (paramIt != m_Params.end())
		{
			return paramIt->second->getKeyword();
		}

		InletMap::const_iterator inletIt = m_Inlets.find(name);
		if (inletIt != m_Inlets.end())
		{
			return inletIt->second->getKeyword();
		}

		std::ostringstream msg;
		msg << "no inlet or setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	void ServiceIO::setValue(std::string const& name, Data const& value)
	{
		ParamMap::iterator paramIt = m_Params.find(name);
		if (paramIt != m_Params.end())
		{
			paramIt->second->setData(value.data());
		}

		InletMap::iterator inletIt = m_Inlets.find(name);
		if (inletIt != m_Inlets.end())
		{
			inletIt->second->setData(TimestampedData(value.getTimestamp(), value.data()));
		}

		std::ostringstream msg;
		msg << "no inlet or setup parameter named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	void ServiceIO::insertValue(std::string const& name, Data const& value)
	{
		InletMap::iterator inletIt = m_Inlets.find(name);
		if (inletIt != m_Inlets.end())
		{
			inletIt->second->insertData(TimestampedData(value.getTimestamp(), value.data()));
		}

		std::ostringstream msg;
		msg << "no inlet named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	InputHandle ServiceIO::createInputHandle(std::string const& name)
	{
		InletMap::iterator it = m_Inlets.find(name);
		if (it != m_Inlets.end())
		{
			return InputHandle(*(it->second));
		}

		std::ostringstream msg;
		msg << "no inlet named " << name << " found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	OutputHandle ServiceIO::createOutputHandle(std::string const& name)
	{
		OutletMap::iterator it = m_Outlets.find(name);
		if (it != m_Outlets.end())
		{
			return  OutputHandle(*(it->second));
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
			it->second->syncBuffers();
		}
	}

	void ServiceIO::updateOutlets()
	{
		for (OutletMap::iterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it)
		{
			it->second->update();
		}
	}

	void ServiceIO::updateInlets()
	{
		for (InletMap::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it)
		{
			it->second->removeConsumedItems();
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