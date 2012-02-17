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

#include "_2RealXMLReader.h"
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

	XMLReader::XMLReader(Poco::Path const& path) :
		m_Filepath(path),
		m_Document()
	{
		Poco::XML::DOMParser p;
		m_Document = p.parse(m_Filepath.toString());
	}

	std::string const& XMLReader::getFilepath() const
	{
		return m_Filepath.toString();
	}

	Poco::XML::Node& XMLReader::getRoot()
	{
		return *m_Document->firstChild();
	}

	const std::string XMLReader::getNodeAttribute(std::string const& name, Poco::XML::Node &node)
	{
		Poco::XML::NamedNodeMap *attributes = node.attributes();
		if (!attributes)
		{
			throw XMLFormatException("xml node \'" + node.nodeName() + "\' has no attributes");
		}

		Poco::XML::Node *attrib = attributes->getNamedItem(name);
		if (!attrib)
		{
			attributes->release();
			throw XMLFormatException("xml node \'" + node.nodeName() + "\' has no attribute \'" + name + "\'");
		}

		const std::string result = attrib->nodeValue();
		attributes->release();
		return result;
	}

	const std::string XMLReader::getChildText(std::string const& name, Poco::XML::Node &node)
	{
		Poco::XML::NodeList *children = node.childNodes();
		Poco::XML::Node *child = NULL;
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Poco::XML::Node *n = children->item(i);
			if (n->nodeType() == Poco::XML::Node::ELEMENT_NODE && n->localName() == name)
			{

				if (child != NULL)
				{
					children->release();
					throw XMLFormatException("xml node \'" + node.nodeName() + "\' has multiple children named \'" + name + "\'");
				}

				child = n;
			}
		}

		if (child == NULL)
		{
			children->release();
			throw XMLFormatException("xml node \'" + node.nodeName() + "\' has no child named \'" + name + "\'");
		}

		std::string result = child->innerText();
		children->release();
		return result;
	}

	Poco::XML::Node & XMLReader::getChildNode(std::string const& name, Poco::XML::Node &node)
	{
		Poco::XML::NodeList *children = node.childNodes();
		Poco::XML::Node *child = NULL;
		for (unsigned int i=0; i<children->length(); ++i)
		{
			Poco::XML::Node *n = children->item(i);
			if (n->nodeType() == Poco::XML::Node::ELEMENT_NODE && n->localName() == name)
			{

				if (child != NULL)
				{
					children->release();
					throw XMLFormatException("xml node \'" + node.nodeName() + "\' has multiple children named \'" + name + "\'");
				}

				child = n;
			}
		}

		if (child == NULL)
		{
			children->release();
			throw XMLFormatException("xml node \'" + node.nodeName() + "\' has no child named \'" + name + "\'");
		}

		children->release();
		return *child;
	}

}