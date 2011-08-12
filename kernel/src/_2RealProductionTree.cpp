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

#include "_2RealTypedefs.h"
#include "_2RealProductionTree.h"
#include "_2RealOutputContainer.h"
#include "_2RealOutputListener.h"

namespace _2Real
{

	ProductionTree::ProductionTree(OutputContainer *const _container) : m_ContainerPtr(_container)
	{
	}

	ProductionTree::ProductionTree(ProductionTree const& _src) : m_ContainerPtr(_src.m_ContainerPtr)
	{
	}

	ProductionTree& ProductionTree::operator=(ProductionTree const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}
 
		m_ContainerPtr = _src.m_ContainerPtr; 
	 
		return *this;
	}

	ProductionTree::~ProductionTree()
	{
		for (NamedListenerMap::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++)
		{
			m_ContainerPtr->removeOutputListener(it->second);
			delete it->second;
		}
		
		delete m_ContainerPtr;
	}

	const bool ProductionTree::beginProductionTree(eContainerType const& _type, ErrorState &_error)
	{
		return m_ContainerPtr->beginProductionTree(_type);
	}

	const bool ProductionTree::beginProductionTree(ErrorState &_error)
	{
		return m_ContainerPtr->beginProductionTree();
	}

	const bool ProductionTree::endProductionTree(ErrorState &_error)
	{
		return m_ContainerPtr->endProductionTree();
	}

	const bool ProductionTree::beginGroup(eContainerType const& _type, ErrorState &_error)
	{
		return m_ContainerPtr->beginGroup(_type);
	}

	const bool ProductionTree::endGroup(ErrorState &_error)
	{
		return m_ContainerPtr->endGroup();
	}

	const bool ProductionTree::beginServiceProductionTree(std::string const& _name, std::string const& _plugin, ErrorState &_error)
	{
		return m_ContainerPtr->beginServiceProductionTree(_name, _plugin);
	}

	const bool ProductionTree::endServiceProductionTree(ErrorState &_error)
	{
		return m_ContainerPtr->endServiceProductionTree();
	}

	const bool ProductionTree::configureSetupParameter(AbstractServiceVariable *_param, ErrorState &_error)
	{
		return m_ContainerPtr->configureSetupParameter(_param);
	}

	const Variable ProductionTree::configureOutputParameter(std::string const& _name, ErrorState &_error)
	{
		return m_ContainerPtr->configureOutputVariable(_name);
	}

	const bool ProductionTree::configureInputParameter(std::string const& _name, Variable const& _var, ErrorState &_error)
	{
		return m_ContainerPtr->configureInputVariable(_name, _var);
	}

	const bool ProductionTree::addOutputListener(std::string const& _name, Variable const& _fwName, OutputListenerCallback _callback, ErrorState &_error)
	{
		OutputListener *listener = new OutputListener(_name, _fwName, 0);
		m_Listeners.insert(NamedListener(_name, listener));
		m_ContainerPtr->addOutputListener(listener);
		return true;
	}

	const bool ProductionTree::removeOutputListener(std::string const& _name, ErrorState &_error)
	{
		NamedListenerMap::iterator it = m_Listeners.find(_name);
		if (it != m_Listeners.end())
		{
			return m_ContainerPtr->removeOutputListener(it->second);
		}

		return false;
	}

	AnyPtr ProductionTree::getData(std::string const& _name, ErrorState &_error)
	{
		NamedListenerMap::iterator it = m_Listeners.find(_name);
		if (it != m_Listeners.end())
		{
			return it->second->getData();
		}

		return AnyPtr();
	}

	const bool hasNewData(std::string const& _name, unsigned long const& _compare, ErrorState &_error)
	{
		return true;
	}

	const bool ProductionTree::updateOnce(bool const& _blocking, ProductionTreeCallback _callback, ErrorState &_error)
	{
		//return m_ContainerPtr->update();
		return true;
	}

	const bool ProductionTree::runSynchronously(ProductionTreeCallback _callback, ErrorState &_error)
	{
		return true;
	}

	const bool ProductionTree::runAsynchronously(ProductionTreeCallback _callback, ErrorState &_error)
	{
		return true;
	}

	const bool ProductionTree::stop(ErrorState &_error)
	{
		return true;
	}

	const bool ProductionTree::setPluginPath(std::string const& _path, ErrorState &_error)
	{
		m_PluginPath = _path;
		return true;
	}
}