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

#pragma once

#include "_2RealException.h"

#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/SAX/SAXException.h"

#include <string>
#include <iostream>

namespace _2Real
{

	class XMLReader
	{

	public:

		XMLReader(std::string const& xmlPath);

		std::string const& getFilepath() const;
		Poco::XML::Node& getRoot();

		static const std::string getNodeAttribute(std::string const& name, Poco::XML::Node &node);
		static const std::string getChildText(std::string const& name, Poco::XML::Node &node);
		static Poco::XML::Node & XMLReader::getChildNode(std::string const& name, Poco::XML::Node &node);

	protected:

		Poco::XML::XMLString					const m_Filepath;
		Poco::AutoPtr< Poco::XML::Document >	m_Document;

	};

}