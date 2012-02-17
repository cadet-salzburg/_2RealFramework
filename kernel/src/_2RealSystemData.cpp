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


	SymbolName::SymbolName(std::string const &name)
	{
		char delim = ':';
		std::istringstream iss(name);
		std::string token;
		while (getline(iss, token, delim))
		{
			push_back(token);
		}
	}

	SymbolName::SymbolName()
	{
	}

	const std::string SymbolName::toString() const
	{
		std::ostringstream str;

		if (empty())
		{
			return "";
		}

		std::list< std::string >::const_iterator it = this->begin();
		str << *it;

		for (std::list< std::string >::const_iterator it = this->begin(); it != this->end(); ++it)
		{
			str << "_" << *it;
		}

		return str.str();
	}

	SymbolName::SymbolName(std::list< std::string > const& symbols) :
		std::list< std::string >((symbols))
	{
	}

	const SymbolName SymbolName::getPrefixes() const
	{
		if (size() < 2)
		{
			return SymbolName();
		}
		else
		{
			std::list< std::string >::const_iterator begin = this->begin();
			std::list< std::string >::const_iterator end = this->end();
			end--;
			std::list< std::string > result(begin, end);
			return SymbolName(result);
		}
	}

	void SymbolName::addPrefixes(SymbolName symbols)
	{
		this->splice(this->begin(), symbols);
	}

	const std::string SymbolName::getLastSymbol() const
	{
		if (empty())
		{
			return "";
		}
		else
		{
			return back();
		}
	}

	void SymbolName::pushSymbol(std::string const& symbol)
	{
		push_back(symbol);
	}

	void SymbolName::popSymbol()
	{
		pop_back();
	}

	void SymbolName::popPrefix()
	{
		pop_front();
	}

	Symbol::Symbol(SymbolName const& name) :
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

	SymbolName const& Symbol::getName() const
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

	ExtendedSymbol::ExtendedSymbol(SymbolName const& name) :
		Symbol(name)
	{
	}

	const std::string ExtendedSymbol::getExtendedName() const
	{
		return m_Name.getLastSymbol();
	}

	const SymbolName ExtendedSymbol::getSymbolName() const
	{
		return m_Name.getPrefixes();
	}

	IOLink::IOLink(SymbolName const& in, SymbolName const& out) :
		m_In(in),
		m_Out(out)
	{
	}

	const SymbolName IOLink::getInServiceName() const
	{
		return m_In.getSymbolName();
	}

	const std::string IOLink::getInSlotName() const
	{
		return m_In.getExtendedName();
	}

	const SymbolName IOLink::getOutServiceName() const
	{
		return m_Out.getSymbolName();
	}

	const std::string IOLink::getOutSlotName() const
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

	PluginData::PluginData(SymbolName const& name, std::string const& classname, Poco::Path const& path) :
		Symbol(name),
		m_Path(path),
		m_Classname(classname)
	{
	}

	std::string const& PluginData::getClassname() const
	{
		return m_Classname;
	}

	Poco::Path const& PluginData::getPluginPath() const
	{
		return m_Path;
	}

	std::list< ParamValue > & PluginData::getSetupInfo()
	{
		return m_SetupInfo;
	}

	ServiceData::ServiceData(SymbolName const& name, float fps, SymbolName const& service) :
		GraphData(name, fps, "service"),
		m_Service(service),
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

	const std::string ServiceData::getServiceName() const
	{
		return m_Service.getExtendedName();
	}

	const SymbolName ServiceData::getPluginName() const
	{
		return m_Service.getSymbolName();
	}

	Identifier const& ServiceData::getPluginId() const
	{
		return m_Service.getId();
	}

	GraphData::GraphData(SymbolName const& name, float fps, std::string const& type) :
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

	ParamValue::ParamValue(SymbolName const& name, std::string const& value) :
		m_Name(name),
		m_Value(value)
	{
	}

	SymbolName const& ParamValue::getName() const
	{
		return m_Name;
	}

	std::string const& ParamValue::getValue() const
	{
		return m_Value;
	}

	////////////////////////////////////////////////////////////////////////////////

	XMLData::XMLData(Poco::Path const& path, SymbolName const& name) :
		m_Reader(path),
		m_NamePrefix(name)
	{
	}

	XMLData::~XMLData()
	{
		for (std::list< GraphData * >::iterator it = m_Graphs.begin(); it != m_Graphs.end();)
		{
			delete *it;
			it = m_Graphs.erase(it);
		}
	}

	std::list< PluginData >& XMLData::getPlugins()
	{
		return m_Plugins;
	}

	std::list< GraphData * >& XMLData::getGraphs()
	{
		return m_Graphs;
	}

	std::list< IOLink >& XMLData::getIOLinks()
	{
		return m_IOLinks;
	}

	std::set< SymbolName >& XMLData::getNames()
	{
		return m_Names;
	}

	SystemData::SystemData(Poco::Path const& path) :
		XMLData(path, SymbolName())
	{
		processSystem();

		std::cout << m_Names << std::endl;

	}

	const Poco::Path SystemData::getDefaultDirectory() const
	{
		return m_DefaultDir;
	}

	const std::string SystemData::getLogfile() const
	{
		return m_Logfile;
	}

	void SystemData::processSystem()
	{
		Node& system = m_Reader.getRoot();
		if (system.localName() != "system")
		{
			throw XMLFormatException("xml element \'system\' missing");
		}

		m_DefaultDir = Poco::Path(trim(m_Reader.getChildText("default_directory", system)));
		m_Logfile = trim(m_Reader.getChildText("logfile", system));

		SymbolName currentPrefix = m_NamePrefix;

		processPluginList(m_Reader.getChildNode("plugin_list", system), m_Plugins, currentPrefix);
		processGraphList(m_Reader.getChildNode("graph_list", system), m_Graphs, currentPrefix);
		processIOLinkList(m_Reader.getChildNode("link_list", system), m_IOLinks, currentPrefix);
	}

	void XMLData::processPluginList(Node const& plugins, std::list< PluginData > &pluginList, SymbolName &prefix)
	{
		NodeList *children = plugins.childNodes();
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *plugin = children->item(i);
			if (plugin->nodeType() == Node::ELEMENT_NODE && plugin->localName() == "plugin")
			{
				std::string name = validateName(m_Reader.getChildText("name", *plugin));

				std::string classname = trim(m_Reader.getChildText("classname", *plugin));

				Poco::Path dir = trim(m_Reader.getChildText("directory", *plugin));

				prefix.pushSymbol(name);
				if (m_Names.find(prefix) != m_Names.end())
				{
					std::ostringstream msg;
					msg << "naming conflict: " << prefix << " is already defined";
					throw XMLFormatException(msg.str());
				}

				PluginData pluginData(prefix, classname, dir);

				Node const& params = m_Reader.getChildNode("parameter_list", *plugin);

				processParameterList(params, pluginData.getSetupInfo(), prefix);

				m_Names.insert(prefix);
				pluginList.push_back(pluginData);
				prefix.popSymbol();
			}
		}

		children->release();
	}

	void XMLData::processParameterList(Node const& params, std::list< ParamValue > &paramList, SymbolName &prefix)
	{
		NodeList *children = params.childNodes();
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *param = children->item(i);
			if (param->nodeType() == Node::ELEMENT_NODE && param->localName() == "parameter")
			{
				std::string name = toLower(trim(m_Reader.getChildText("name", *param)));

				std::string value = trim(m_Reader.getChildText("value", *param));

				prefix.pushSymbol(name);
				ParamValue p(prefix, value);
				prefix.popSymbol();

				paramList.push_back(p);
			}
		}

		children->release();
	}

	void XMLData::processGraphList(Node const& graphs, std::list< GraphData * > &graphList, SymbolName &prefix, bool replace)
	{
		NodeList *children = graphs.childNodes();

		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *graph = children->item(i);
			if (graph->nodeType() == Node::ELEMENT_NODE && graph->localName() == "graph")
			{
				std::string type = toLower(trim(m_Reader.getNodeAttribute("type", *graph)));

				std::string name = validateName(m_Reader.getChildText("name", *graph));

				std::string fpsStr = trim(m_Reader.getChildText("fps", *graph));
				float fps = (float)::atof(fpsStr.c_str());

				if (!replace)
				{
					prefix.pushSymbol(name);
				}
				if (m_Names.find(prefix) != m_Names.end())
				{
					std::ostringstream msg;
					msg << "naming conflict: " << prefix << " is already defined";
					throw XMLFormatException(msg.str());
				}

				if (type == "service")
				{
					std::string service = toLower(trim(m_Reader.getChildText("service", *graph)));

					SymbolName serviceName(service);
					serviceName.addPrefixes(m_NamePrefix);

					//don't know if this service exists yet
					ServiceData *data = new ServiceData(prefix, fps, serviceName);
					//but can check the plugin
					if (m_Names.find(data->getPluginName()) == m_Names.end())
					{
						std::ostringstream msg;
						msg << "undefined plugin symbol found: " << data->getPluginName();
						throw XMLFormatException(msg.str());
					}

					Node const& params = m_Reader.getChildNode("parameter_list", *graph);
					processParameterList(params, data->getSetupInfo(), prefix);

					graphList.push_back(data);
				}
				else if (type == "sequence" || type == "synchronization")
				{
					GraphData *data = new GraphData(prefix, fps, type);

					Node const& childGraphs = m_Reader.getChildNode("graph_list", *graph);
					processGraphList(childGraphs, data->getChildren(), prefix);

					graphList.push_back(data);
				}
				else if (type == "external_block")
				{
					std::string source = trim(m_Reader.getChildText("source", *graph));

					Poco::Path path(source);
					//TODO: path
					path.absolute();

					BlockData block(source, prefix);

					std::list< PluginData > &plugins = block.getPlugins();
					std::list< GraphData * > &graphs = block.getGraphs();
					std::list< IOLink > &links = block.getIOLinks();
					std::set< SymbolName > names = block.getNames();

					graphList.splice(graphList.begin(), graphs);
					m_Plugins.splice(m_Plugins.begin(), plugins);
					m_IOLinks.splice(m_IOLinks.begin(), links);

					for (std::set< SymbolName >::iterator it = names.begin(); it != names.end(); ++it)
					{
						if (!m_Names.insert(*it).second)
						{
							std::ostringstream msg;
							msg << "naming conflict when reading external block: " << *it << " is already defined";
							throw XMLFormatException(msg.str());
						}
					}

					//const unsigned int sz1 = m_Names.size();
					//const unsigned int sz2 = names.size();

					//m_Names.insert(names.begin(), names.end());
					//if (m_Names.size() != (sz1+sz2))
					//{
					//	std::ostringstream msg;
					//	msg << "naming conflict when reading external block " << source;
					//	throw XMLFormatException(msg.str());
					//}
				}
				else
				{
					throw XMLFormatException("invalid graph type " + type);
				}

				m_Names.insert(prefix);
				prefix.popSymbol();
			}
		}

		children->release();
	}

	void XMLData::processIOLinkList(Node const& links, std::list< IOLink > &linkList, SymbolName &prefix, bool replace)
	{
		NodeList *children = links.childNodes();
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Node *link = children->item(i);
			if (link->nodeType() == Node::ELEMENT_NODE && link->localName() == "link")
			{
				std::string in = toLower(trim(m_Reader.getChildText("in", *link)));
				std::string out = toLower(trim(m_Reader.getChildText("out", *link)));

				SymbolName linkIn(in);

				if (replace)
				{
					linkIn.popPrefix();
				}

				linkIn.addPrefixes(m_NamePrefix);
				SymbolName linkOut(out);
				
				if (replace)
				{
					linkOut.popPrefix();
				}

				linkOut.addPrefixes(m_NamePrefix);

				IOLink linkData(linkIn, linkOut);

				if (m_Names.find(linkData.getInServiceName()) == m_Names.end())
				{
					std::ostringstream msg;
					msg << "undefined symbol in IO Link found: " << linkData.getInServiceName();
					throw XMLFormatException(msg.str());
				}
				if (m_Names.find(linkData.getOutServiceName()) == m_Names.end())
				{
					std::ostringstream msg;
					msg << "undefined symbol in IO Link found: " << linkData.getOutServiceName();
					throw XMLFormatException(msg.str());
				}


				linkList.push_back(linkData);
			}
		}

		children->release();
	}

	BlockData::BlockData(Poco::Path const& path, SymbolName const& name) :
		XMLData(path, name)
	{
		processBlock();
	}

	void BlockData::processBlock()
	{
		Node& system = m_Reader.getRoot();
		if (system.localName() != "block")
		{
			throw XMLFormatException("xml element \'block\' missing");
		}

		SymbolName currentPrefix = m_NamePrefix;

		processPluginList(m_Reader.getChildNode("plugin_list", system), m_Plugins, currentPrefix);
		processGraphList(m_Reader.getChildNode("graph_list", system), m_Graphs, currentPrefix, true);
		processIOLinkList(m_Reader.getChildNode("link_list", system), m_IOLinks, currentPrefix, true);
	}

}