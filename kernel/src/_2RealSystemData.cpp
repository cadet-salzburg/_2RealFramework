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

#include "_2RealSystemData.h"
#include "_2RealException.h"

#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/Treewalker.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/AutoPtr.h"

#include <sstream>

using namespace Poco::XML;

namespace _2Real
{

	Symbol::Symbol(std::string const& name) :
		m_Name(name),
		m_Id()
	{
	}

	Symbol::~Symbol()
	{
	}

	const bool Symbol::isResolved() const
	{
		return m_Id.isValid();
	}

	std::string const& Symbol::getName() const
	{
		return m_Name;
	}

	Identifier const& Symbol::getId() const
	{
		return m_Id;
	}

	void Symbol::resolveTo(Identifier const& id)
	{
		m_Id = id;
	}

	ExtendedSymbol::ExtendedSymbol(std::string const& name, std::string const& symbolName) :
		Symbol(symbolName),
		m_ExtendedName(name)
	{
	}

	std::string const& ExtendedSymbol::getExtendedName() const
	{
		return m_ExtendedName;
	}

	IOLink::IOLink(std::string const& serviceIn, std::string const& input, std::string const& serviceOut, std::string const& output) :
		m_In(input, serviceIn),
		m_Out(output, serviceOut)
	{
	}

	std::string const& IOLink::getInServiceName() const
	{
		return m_In.getName();
	}

	std::string const& IOLink::getInSlotName() const
	{
		return m_In.getExtendedName();
	}

	std::string const& IOLink::getOutServiceName() const
	{
		return m_Out.getName();
	}

	std::string const& IOLink::getOutSlotName() const
	{
		return m_Out.getExtendedName();
	}

	Identifier const& IOLink::getInputId() const
	{
		return m_In.getId();
	}

	Identifier const& IOLink::getOutputId() const
	{
		return m_Out.getId();
	}

	void IOLink::resolveInputService(Identifier const& serviceId)
	{
		m_In.resolveTo(serviceId);
	}

	void IOLink::resolveOutputService(Identifier const& serviceId)
	{
		m_Out.resolveTo(serviceId);
	}

	const bool IOLink::isFullyResolved() const
	{
		return m_In.isResolved() && m_Out.isResolved();
	}

	PluginData::PluginData(std::string const& name, std::string const& classname) :
		Symbol(name),
		m_Classname(classname)
	{
	}

	std::string const& PluginData::getClassname() const
	{
		return m_Classname;
	}

	std::list< ParamValue > & PluginData::getSetupInfo()
	{
		return m_SetupInfo;
	}

	ServiceData::ServiceData(std::string const& name, float fps, std::string const& service, std::string const& plugin) :
		GraphData(name, fps, "service"),
		m_Service(service, plugin),
		m_SetupInfo()
	{
	}

	const bool ServiceData::isFullyResolved() const
	{
		return isResolved() && m_Service.isResolved();
	}

	std::list< ParamValue > & ServiceData::getSetupInfo()
	{
		return m_SetupInfo;
	}

	void ServiceData::resolvePlugin(Identifier const& pluginId)
	{
		m_Service.resolveTo(pluginId);
	}

	std::string const& ServiceData::getServiceName() const
	{
		return m_Service.getExtendedName();
	}

	std::string const& ServiceData::getPluginName() const
	{
		return m_Service.getName();
	}

	Identifier const& ServiceData::getPluginId() const
	{
		return m_Service.getId();
	}

	GraphData::GraphData(std::string const& name, float fps, std::string const& type) :
		Symbol(name),
		m_Fps(fps),
		m_Type(type),
		m_Children()
	{
	}

	GraphData::~GraphData()
	{
		for (std::list< GraphData * >::iterator it = m_Children.begin(); it != m_Children.end();)
		{
			delete *it;
			it = m_Children.erase(it);
		}
	}

	const bool GraphData::isFullyResolved() const
	{
		return true;
	}

	std::list< GraphData * > & GraphData::getChildren()
	{
		return m_Children;
	}

	const float GraphData::getFps() const
	{
		return m_Fps;
	}

	std::string const& GraphData::getType() const
	{
		return m_Type;
	}

	ParamValue::ParamValue(std::string const& name, std::string const& value) :
		m_Name(name),
		m_Value(value)
	{
	}

	std::string const& ParamValue::getName() const
	{
		return m_Name;
	}

	std::string const& ParamValue::getValue() const
	{
		return m_Value;
	}

	SystemData::SystemData(std::string const& xmlPath) :
		m_Reader(xmlPath)
	{
		processSystem();
	}

	SystemData::~SystemData()
	{
		for (std::list< GraphData * >::iterator it = m_Graphs.begin(); it != m_Graphs.end();)
		{
			delete *it;
			it = m_Graphs.erase(it);
		}
	}

	const std::string SystemData::getDefaultDirectory() const
	{
		return m_DefaultDir;
	}

	const std::string SystemData::getLogfile() const
	{
		return m_Logfile;
	}

	std::list< PluginData >& SystemData::getPlugins()
	{
		return m_Plugins;
	}

	std::list< GraphData * >& SystemData::getGraphs()
	{
		return m_Graphs;
	}

	std::list< IOLink >& SystemData::getIOLinks()
	{
		return m_IOLinks;
	}

	void SystemData::processSystem()
	{
		Node& system = m_Reader.getRoot();
		if (system.localName() != "system")
		{
			throw XMLFormatException("xml element \'system\' missing");
		}

		m_DefaultDir = trim(m_Reader.getChildText("default_directory", system));
		m_Logfile = trim(m_Reader.getChildText("logfile", system));

		TreeWalker walker(&system, NodeFilter::SHOW_ELEMENT);
		Node *current = walker.currentNode();

		if ((current = walker.firstChild()) == NULL)
		{
			throw XMLFormatException("xml system configuration is empty");
		}

		while (current != NULL)
		{
			if (current->localName() == "plugin_list")
			{
				processPluginList(*current, m_Plugins, "");
			}
			else if (current->localName() == "graph_list")
			{
				processGraphList(*current, m_Graphs, "");
			}
			else if (current->localName() == "link_list")
			{
				processIOLinkList(*current, m_IOLinks, "");
			}
			current = walker.nextSibling();
		}
	}

	void SystemData::processPluginList(Node const& plugins, std::list< PluginData > &pluginList, std::string const& prefix)
	{
		std::cout << "processing list of plugins" << std::endl;

		NodeList *children = plugins.childNodes();
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *plugin = children->item(i);
			if (plugin->nodeType() == Node::ELEMENT_NODE && plugin->localName() == "plugin")
			{
				std::string name = validateName(m_Reader.getChildText("name", *plugin));
				std::cout << "name " << name << std::endl;

				std::string classname = trim(m_Reader.getChildText("classname", *plugin));
				std::cout << "classname " << classname << std::endl;

				PluginData pluginData(name, classname);
				Node const& params = m_Reader.getChildNode("parameter_list", *plugin);
				processParameterList(params, pluginData.getSetupInfo(), "");

				pluginList.push_back(pluginData);
			}
		}

		children->release();
	}

	void SystemData::processParameterList(Node const& params, std::list< ParamValue > &paramList, std::string const& prefix)
	{
		std::cout << "processing list of params" << std::endl;

		NodeList *children = params.childNodes();
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *param = children->item(i);
			std::cout << "URI " << param->namespaceURI() << std::endl;
			if (param->nodeType() == Node::ELEMENT_NODE && param->localName() == "parameter")
			{
				std::string name = toLower(trim(m_Reader.getChildText("name", *param)));
				std::cout << "name " << name << std::endl;

				std::string value = trim(m_Reader.getChildText("value", *param));
				std::cout << "value " << value << std::endl;

				ParamValue p(name, value);
				paramList.push_back(p);
			}
		}

		children->release();
	}

	void SystemData::processGraphList(Node const& graphs, std::list< GraphData * > &graphList, std::string const& prefix)
	{
		std::cout << "processing list of graphs" << std::endl;

		NodeList *children = graphs.childNodes();
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *graph = children->item(i);
			if (graph->nodeType() == Node::ELEMENT_NODE && graph->localName() == "graph")
			{
				std::string type = toLower(trim(m_Reader.getNodeAttribute("type", *graph)));
				std::cout << "type " << type << std::endl;

				std::string name = prefix + validateName(m_Reader.getChildText("name", *graph));
				std::cout << "name " << name << std::endl;

				std::string fpsStr = trim(m_Reader.getChildText("fps", *graph));
				float fps = (float)::atof(fpsStr.c_str());
				std::cout << "fps " << fps << std::endl;

				if (type == "service")
				{
					std::string plugin = toLower(trim(m_Reader.getChildText("plugin", *graph)));
					std::cout << "plugin " << plugin << std::endl;

					std::string service = toLower(trim(m_Reader.getChildText("service", *graph)));
					std::cout << "service " << service << std::endl;

					ServiceData *data = new ServiceData(name, fps, service, plugin);

					Node const& params = m_Reader.getChildNode("parameter_list", *graph);
					processParameterList(params, data->getSetupInfo(), "");

					graphList.push_back(data);
				}
				else if (type == "sequence" || type == "synchronization")
				{
					GraphData *data = new GraphData(name, fps, type);

					Node const& childGraphs = m_Reader.getChildNode("graph_list", *graph);
					processGraphList(childGraphs, data->getChildren(), prefix);

					graphList.push_back(data);
				}
				else if (type == "external")
				{
					std::string source = trim(m_Reader.getChildText("source", *graph));
					std::cout << "source " << source << std::endl;

					GraphData *data = processExternalGraph(name, fps, source, prefix);

					graphList.push_back(data);
				}
				else
				{
					throw XMLFormatException("invalid graph type " + type);
				}
			}
		}

		children->release();
	}

	void SystemData::processIOLinkList(Node const& links, std::list< IOLink > &linkList, std::string const& prefix)
	{
		std::cout << "processing list of links" << std::endl;

		NodeList *children = links.childNodes();
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *link = children->item(i);
			if (link->nodeType() == Node::ELEMENT_NODE && link->localName() == "link")
			{
				std::string in_graph = toLower(trim(m_Reader.getChildText("in_graph", *link)));
				std::cout << "in_graph" << in_graph << std::endl;

				std::string in_slot = toLower(trim(m_Reader.getChildText("in_slot", *link)));
				std::cout << "in_slot " << in_slot << std::endl;

				std::string out_graph = toLower(trim(m_Reader.getChildText("out_graph", *link)));
				std::cout << "out_graph" << out_graph << std::endl;

				std::string out_slot = toLower(trim(m_Reader.getChildText("out_slot", *link)));
				std::cout << "out_slot " << out_slot << std::endl;

				IOLink linkData(in_graph, in_slot, out_graph, out_slot);
				linkList.push_back(linkData);
			}
		}

		children->release();
	}

	GraphData * SystemData::processExternalGraph(std::string const& name, float fps, std::string const& source, std::string const& prefix)
	{
		XMLReader graphReader(source);
		Node &graph = graphReader.getRoot();

		std::string type = m_Reader.getNodeAttribute("type", graph);

		if (type == "sequence" || type == "synchronization")
		{
			GraphData *data = new GraphData(name, fps, type);

			//process the plugins....
			Node const& plugins = m_Reader.getChildNode("plugin_list", graph);

			processPluginList(plugins, m_Plugins, prefix+name+" : ");

			Node const& childGraphs = m_Reader.getChildNode("graph_list", graph);
			processGraphList(childGraphs, data->getChildren(), prefix+name+" : ");
			return data;
		}
		
		return NULL;
	}

}