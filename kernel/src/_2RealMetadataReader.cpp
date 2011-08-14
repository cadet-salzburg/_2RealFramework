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
#include "_2RealMetaData.h"

#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/AutoPtr.h"

#include <fstream>

namespace _2Real
{

	Metadata const *const MetadataReader::readXMLFile(std::string const& _file)
	{
		//DOMParser parser;
		//AutoPtr<Document> document = parser.parse(XMLString(_file));
		//NodeIterator it(document, NodeFilter::SHOW_ELEMENT);
		//Node* node = it.nextNode();

		//while (node)
		//{
		//	if (node->nodeName() == "plugin" && node->hasAttributes())
		//	{
		//		NamedNodeMap* map = node->attributes();
		//		Node* attrib = map->getNamedItem("name");
		//		
		//		attrib->release();
		//		map->release();
		//	}		
		//	
		//	node = it.nextNode();
		//}

		return NULL;
	}
}