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
#include "_2RealPluginMetadata.h"
#include "_2RealEngineImpl.h"
#include "_2RealException.h"
#include "_2RealPlugin.h"

#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/AutoPtr.h"

#include <fstream>

using namespace Poco::XML;

namespace _2Real
{

	void MetadataReader::readMetadata(PluginMetadata &_info)
	{

		std::string path = _info.getInstallDirectory() + _info.getClassname() + ".xml";

#ifdef _DEBUG
		std::cout << "metadata reader: reading xml file: " << path << std::endl;
#endif

		try
		{

			DOMParser parser;
			AutoPtr<Document> document = parser.parse(XMLString(path));
			NodeIterator it(document, NodeFilter::SHOW_ELEMENT);
			Node* node = it.nextNode();

			PluginMetadata *result = NULL;

			NodeList *list = document->getElementsByTagName("plugin info");
			
			std::cout << list->length() << std::endl;

			//while (node)
			//{
			//	if (node->nodeName() == "plugin info" && node->hasAttributes())
			//	{
			//		NamedNodeMap* map = node->attributes();
			//		Node* attrib = map->getNamedItem("name");

			//		//create metadata
			//		result = new PluginMetadata(attrib->nodeValue());

			//		attrib->release();
			//		map->release();
			//	}

			//	node = it.nextNode();
			//}
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

}