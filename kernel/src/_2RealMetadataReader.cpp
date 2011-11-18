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
#include "_2RealTypes.h"

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

	const std::string MetadataReader::getNodeAttribute(std::string const& _name, Node *const _node)
	{
		NamedNodeMap *attribs = _node->attributes();
		if (!attribs)
		{
			throw MetadataFormatException("xml node \'" + _node->nodeName() + "\' has no attributes");
		}

		Node *attrib = attribs->getNamedItem(_name);
		if (!attrib)
		{
			attribs->release();
			throw MetadataFormatException("xml node \'" + _node->nodeName() + "\' has no attribute \'" + _name + "\'");
		}

		if (_name == "type" && !m_Metadata.getTypes().contains(attrib->nodeValue()))
		{
			throw MetadataFormatException("xml node \'" + _node->nodeName() + "\' has unknown type \'" + attrib->nodeValue() + "\'");
		}

		return attrib->nodeValue();
	}

	const std::string MetadataReader::getParameterType(Node *const _attrib)
	{
		std::string type = _attrib->nodeValue();
		return m_Metadata.getTypes().getTypename(type);
	}


	Node *const MetadataReader::getChildNode(std::string const& _name, Node *const _parent)
	{
		NodeList *children = _parent->childNodes();
		
		if (!children)
		{
			throw MetadataFormatException("xml node \'" + _parent->localName() + "\' has no child node named \'" + _name + "\'");
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
		if (_info.getClassname() != getNodeAttribute("name", _plugin))
		{
			throw MetadataFormatException("attribute \'name\' of plugin does not match classname");
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
					_info.addSetupParameter(name, type);
				}
			}
		}

		setup->release();
	}

	void MetadataReader::processServiceNode(PluginMetadata &_info, Node *const _service)
	{
		ServiceMetadata meta(getNodeAttribute("name", _service));
		meta.setDescription(getNodeAttribute("description", _service));

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
					meta.addSetupParameter(name, type);
				}
			}
		}

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
					meta.addInputParameter(name, type);
				}
			}
		}

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
					meta.addOutputParameter(name, type);
				}
			}
		}

		_info.addServiceMetadata(meta);
	}

	MetadataReader::MetadataReader(PluginMetadata &_info) :
		m_Metadata(_info)
	{
	}

	void MetadataReader::readMetadata()
	{
		std::string path = m_Metadata.getInstallDirectory() + m_Metadata.getClassname() + ".xml";

		DOMParser parser;
		AutoPtr<Document> document = parser.parse(XMLString(path));

		NodeList *plugins = document->getElementsByTagName("plugin");
		if (!plugins)
		{
			throw MetadataFormatException("xml element \'plugin\' not found");
		}
		else if (plugins->length() > 1)
		{
			throw MetadataFormatException("xml element \'plugin\' should be unique");
		}

		processPluginNode(m_Metadata, plugins->item(0));

		NodeList *services = document->getElementsByTagName("service");
		if (!services)
		{
			throw MetadataFormatException("xml element \'service\' not found");
		}

		for (unsigned long i=0; i<services->length(); i++)
		{
			processServiceNode(m_Metadata, services->item(i));
		}
	}

}