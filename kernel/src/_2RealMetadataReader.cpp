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

#include "_2RealMetaDataReader.h"
#include "_2RealServiceMetadata.h"
#include "_2RealPluginMetadata.h"
#include "_2RealEngine.h"
#include "_2RealException.h"
#include "_2RealPlugin.h"
#include "_2RealTypeTable.h"

#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/AutoPtr.h"

#include <sstream>
#include <iostream>
#include <typeinfo>

using namespace Poco::XML;

namespace _2Real
{
	std::map< std::string, std::string > MetadataReader::s_Typenames = TypeTable().getTable();

	const std::string MetadataReader::getNodeAttribute(std::string const& _name, Node *const _node)
	{
		try
		{
			NamedNodeMap *attribs = _node->attributes();
			if (!attribs)
			{
				throw Exception("metadata format error: element " + _node->nodeName() + "has no attributes");
			}

			Node *attrib = attribs->getNamedItem(_name);
			if (!attrib)
			{
				attribs->release();
				throw Exception("metadata format error: node " + _node->nodeName() + " lacks attribute " + _name);
			}

			std::string result = _name;
			if (_name == "type")
			{
				result = getParameterType(attrib);
			}
			else
			{
				result = attrib->nodeValue();
			}

			return result;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const std::string MetadataReader::getParameterType(Node *const _attrib)
	{
		try
		{
			std::string type = _attrib->nodeValue();
			return getTypename(type);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const std::string MetadataReader::getTypename(std::string const &_type) const
	{
		std::map< std::string, std::string >::const_iterator it = s_Typenames.find(_type);
		if (it == s_Typenames.end())
		{
			throw Exception("metadata format error: unknown parameter type");
		}

		return it->second;
	}

	Node *const MetadataReader::getChildNode(std::string const& _name, Node *const _parent)
	{
		NodeList *children = _parent->childNodes();
		
		if (!children)
		{
			throw Exception("metadata format error: xml node " + _parent->localName() + " has no child nodes");
		}

		for (unsigned long i=0; i<children->length(); i++)
		{
			Node *child = children->item(i);
			if (child->localName() == _name)
			{
				children->release();
				return child;
			}
		}

		children->release();
		return NULL;
	}

	void MetadataReader::processPluginNode(PluginMetadata &_info, Node *const _plugin)
	{
		try
		{
			if (_info.getClassname() != getNodeAttribute("name", _plugin))
			{
				throw Exception("metadata format error: plugin's name must match class name");
			}

			_info.setAuthor(getNodeAttribute("author", _plugin));
			_info.setContact(getNodeAttribute("contact", _plugin));
			_info.setDescription(getNodeAttribute("description", _plugin));

			std::string version = getNodeAttribute("version", _plugin);
			std::stringstream tmp;
			tmp << version;
			unsigned int major, minor, revision;
			unsigned char sep;
			tmp >> major >> sep >> minor >> sep >> revision;
			PluginMetadata::Version v(major, minor, revision);
			_info.setVersion(v);

			Node *setup = getChildNode("setup", _plugin);
			if (setup->hasChildNodes())
			{
				NodeList *children = setup->childNodes();
				for (unsigned long i=0; i<children->length(); i++)
				{
					Node* param = children->item(i);
					if (param->localName() == "param")
					{
						std::string name = getNodeAttribute("name", param);
						std::string type = getNodeAttribute("type", param);
						_info.addSetupParam(name, type);
					}
				}
			}
			setup->release();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void MetadataReader::processServiceNode(PluginMetadata &_info, Node *const _service)
	{
		try
		{
			ServiceMetadata meta(getNodeAttribute("name", _service));
			meta.setDescription(getNodeAttribute("description", _service));

			//std::string singleton = getAttribute("singleton", _service);
			//std::string reconfigurable = getAttribute("reconfigurable", _service);

			Node *setup = getChildNode("setup", _service);
			if (setup->hasChildNodes())
			{
				NodeList *children = setup->childNodes();
				for (unsigned long i=0; i<children->length(); i++)
				{
					Node* param = children->item(i);
					if (param->localName() == "param")
					{
						std::string name = getNodeAttribute("name", param);
						std::string type = getNodeAttribute("type", param);
						meta.addSetupParam(name, type);
					}
				}
			}
			//setup->release();

			Node *input = getChildNode("input", _service);
			if (input->hasChildNodes())
			{
				NodeList *children = input->childNodes();
				for (unsigned long i=0; i<children->length(); i++)
				{
					Node* param = children->item(i);
					if (param->localName() == "param")
					{
						std::string name = getNodeAttribute("name", param);
						std::string type = getNodeAttribute("type", param);
						meta.addInputParam(name, type);
					}
				}
			}
			//input->release();

			Node *output = getChildNode("output", _service);
			if (output->hasChildNodes())
			{
				NodeList *children = output->childNodes();
				for (unsigned long i=0; i<children->length(); i++)
				{
					Node* param = children->item(i);
					if (param->localName() == "param")
					{
						std::string name = getNodeAttribute("name", param);
						std::string type = getNodeAttribute("type", param);
						meta.addOutputParam(name, type);
					}
				}
			}
			//output->release();

			_info.addServiceMetadata(meta);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void MetadataReader::readMetadata(PluginMetadata &_info)
	{
		try
		{
			std::string path = _info.getInstallDirectory() + _info.getClassname() + ".xml";

			DOMParser parser;
			AutoPtr<Document> document = parser.parse(XMLString(path));

			NodeList *plugins = document->getElementsByTagName("plugin");
			if (!plugins || plugins->length() > 1)
			{
				throw Exception("metadata format error: plugin element must be defined exactly once");
			}

			processPluginNode(_info, plugins->item(0));

			NodeList *services = document->getElementsByTagName("service");
			if (!services)
			{
				throw Exception("metadata format error: plugin metadata must define at least one service");
			}

			for (unsigned long i=0; i<services->length(); i++)
			{
				processServiceNode(_info, services->item(i));
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

}