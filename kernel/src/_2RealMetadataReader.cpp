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
#include "_2RealParameterMetadata.h"
#include "_2RealException.h"

#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/AutoPtr.h"

using namespace Poco::XML;

namespace _2Real
{

	const std::string MetadataReader::getNodeAttribute(std::string const& name, Node &node)
	{
		NamedNodeMap *attributes = node.attributes();
		if (!attributes)
		{
			throw XMLFormatException("xml node \'" + node.nodeName() + "\' has no attributes");
		}

		Node *attrib = attributes->getNamedItem(name);
		if (!attrib)
		{
			attributes->release();
			throw XMLFormatException("xml node \'" + node.nodeName() + "\' has no attribute \'" + name + "\'");
		}

		return attrib->nodeValue();
	}

	Node & MetadataReader::getChildNode(std::string const& name, Node &parent)
	{
		NodeList *children = parent.childNodes();
		
		if (!children)
		{
			throw XMLFormatException("xml node \'" + parent.localName() + "\' has no child nodes");
		}

		for (unsigned long i=0; i<children->length(); i++)
		{
			Node *child = children->item(i);
			if (child->localName() == name)
			{
				children->release();
				return *child;
			}
		}

		throw XMLFormatException("xml node \'" + parent.localName() + "\' has no child node named \'" + name + "\'");
	}

	void MetadataReader::processPluginNode(Node &plugin)
	{

		if (m_Metadata.getClassname() != getNodeAttribute("name", plugin))
		{
			throw XMLFormatException("attribute \'name\' of plugin does not match classname");
		}

		m_Metadata.setAuthor(getNodeAttribute("author", plugin));
		m_Metadata.setContact(getNodeAttribute("contact", plugin));
		m_Metadata.setDescription(getNodeAttribute("description", plugin));

		std::string version = getNodeAttribute("version", plugin);
		std::stringstream tmp;
		tmp << version;
		unsigned int major, minor, revision;
		unsigned char sep;
		tmp >> major >> sep >> minor >> sep >> revision;
		Version v(major, minor, revision);
		m_Metadata.setVersion(v);

		Node &setup = getChildNode("setup", plugin);
		if (setup.hasChildNodes())
		{
			NodeList *children = setup.childNodes();
			for (unsigned long i=0; i<children->length(); i++)
			{
				Node* param = children->item(i);
				if (param->localName() == "param")
				{
					std::string name = getNodeAttribute("name", *param);
					std::string type = getNodeAttribute("type", *param);
					m_Metadata.addSetupParameterByKey(name, type);
				}
			}
		}
	}

	void MetadataReader::processServiceNode(Node &service)
	{
		std::string serviceName = getNodeAttribute("name", service);
		m_Metadata.addServiceMetadata(serviceName);
		m_Metadata.setDescription(serviceName, getNodeAttribute("description", service));

		Node &setup = getChildNode("setup", service);
		if (setup.hasChildNodes())
		{
			NodeList *children = setup.childNodes();
			for (unsigned long i=0; i<children->length(); i++)
			{
				Node* param = children->item(i);
				if (param->localName() == "param")
				{
					std::string name = getNodeAttribute("name", *param);
					std::string type = getNodeAttribute("type", *param);
					m_Metadata.addSetupParameterByKey(serviceName, name, type);
				}
			}
		}

		Node &input = getChildNode("input", service);
		if (input.hasChildNodes())
		{
			NodeList *children = input.childNodes();
			for (unsigned long i=0; i<children->length(); i++)
			{
				Node* param = children->item(i);
				if (param->localName() == "param")
				{
					std::string name = getNodeAttribute("name", *param);
					std::string type = getNodeAttribute("type", *param);
					m_Metadata.addInputSlotByKey(serviceName, name, type);
				}
			}
		}

		Node &output = getChildNode("output", service);
		if (output.hasChildNodes())
		{
			NodeList *children = output.childNodes();
			for (unsigned long i=0; i<children->length(); i++)
			{
				Node* param = children->item(i);
				if (param->localName() == "param")
				{
					std::string name = getNodeAttribute("name", *param);
					std::string type = getNodeAttribute("type", *param);
					m_Metadata.addOutputSlotByKey(serviceName, name, type);
				}
			}
		}
	}

	MetadataReader::MetadataReader(PluginMetadata &info) :
		m_Metadata(info)
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
			throw XMLFormatException("xml element \'plugin\' not found");
		}
		else if (plugins->length() > 1)
		{
			throw XMLFormatException("xml element \'plugin\' must be unique");
		}

		processPluginNode(*(plugins->item(0)));

		NodeList *services = document->getElementsByTagName("service");
		if (!services)
		{
			throw XMLFormatException("xml element \'service\' not found - every plugin must export at least one service");
		}

		for (unsigned long i=0; i<services->length(); i++)
		{
			processServiceNode(*(services->item(i)));
		}
	}

}