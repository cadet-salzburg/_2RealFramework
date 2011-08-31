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

#include <sstream>

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

			Node *plugin, *service, *name, *author, *description, *version, *contact, *reconfigure, *singleton;
			NodeList *plugins, *services;
			NamedNodeMap *attribs;
			DOMParser parser;

			AutoPtr<Document> document = parser.parse(XMLString(path));

			//search for plugin tag
			plugins = document->getElementsByTagName("plugin");
			if (plugins == NULL || plugins->length() > 1)
			{
				throw Exception::failure();
			}

			//get attribs
			plugin = plugins->item(0);
			attribs = plugin->attributes();

			//check if plugin name matches name in metadata
			name = attribs->getNamedItem("name");
			if (_info.getClassname() != name->nodeValue())
			{
				throw Exception::failure();
			}

			author = attribs->getNamedItem("author");
			_info.setAuthor(author->nodeValue());

			contact = attribs->getNamedItem("contact");
			_info.setContact(contact->nodeValue());

			description = attribs->getNamedItem("description");
			_info.setDescription(description->nodeValue());

			version = attribs->getNamedItem("version");
			std::stringstream tmp;
			tmp << version->nodeValue();
			unsigned int major, minor, revision;
			unsigned char sep;
			tmp >> major >> sep >> minor >> sep >> revision;
			PluginMetadata::Version v(major, minor, revision);
			_info.setVersion(v);

			//get services
			services = document->getElementsByTagName("service");
			if (services == NULL)
			{
				throw Exception::failure();
			}

			//create service metadata
			for (unsigned int i=0; i<services->length(); i++)
			{

				service = services->item(i);
				attribs = service->attributes();

				name = attribs->getNamedItem("name");
				ServiceMetadata meta(name->nodeValue());

#ifdef _DEBUG
				std::cout << "plugin defines service: " << meta.getName() << std::endl;
#endif

				description = attribs->getNamedItem("description");
				meta.setDescription(description->nodeValue());

				singleton = attribs->getNamedItem("singleton");
				if (singleton->nodeValue() == "true")
				{
					meta.setSingleton(true);
				}
				else
				{
					meta.setSingleton(false);
				}

				reconfigure = attribs->getNamedItem("reconfigurable");
				if (reconfigure->nodeValue() == "true")
				{
					meta.setReconfiguration(true);
				}
				else
				{
					meta.setReconfiguration(false);
				}

				//setup params
				NodeList *children = service->childNodes();
				if (children == NULL)
				{
					throw Exception::failure();
				}

				for (unsigned int j = 0; j < children->length(); j++)
				{
					Node *child = children->item(j);
					XMLString childTag = child->localName();

					if (childTag == "userclasses")
					{
						NodeList *classes = child->childNodes();
						if (classes->length() != NULL)
						{
							for (unsigned int k = 0; k < classes->length(); k++)
							{
								Node *userclass = classes->item(k);
								if (userclass->localName() == "class")
								{
									NamedNodeMap* classAttribs = userclass->attributes();
									Node* classname = classAttribs->getNamedItem("name");
#ifdef _DEBUG
	std::cout << "service defines userclass: " << classname->nodeValue() << std::endl;
#endif
								}
							}
						}
					}
					else if (childTag == "setup")
					{
						NodeList *params = child->childNodes();
						if (params->length() != NULL)
						{
							for (unsigned int k = 0; k<params->length(); k++)
							{
								Node* param = params->item(k);
								if (param->localName() == "param")
								{
									NamedNodeMap* classAttribs = param->attributes();
									Node* paramname = classAttribs->getNamedItem("name");
									Node* paramtype = classAttribs->getNamedItem("type");
									meta.addSetupParam(paramname->nodeValue(), paramtype->nodeValue());

#ifdef _DEBUG
									std::cout << "service has setup param: " << paramname->nodeValue() << " " << paramtype->nodeValue() << std::endl;
#endif
								}
							}
						}
					}
					else if (childTag == "input")
					{
						NodeList *params = child->childNodes();
						if (params->length() != NULL)
						{
							for (unsigned int k = 0; k<params->length(); k++)
							{
								Node* param = params->item(k);
								if (param->localName() == "param")
								{
									NamedNodeMap* classAttribs = param->attributes();
									Node* paramname = classAttribs->getNamedItem("name");
									Node* paramtype = classAttribs->getNamedItem("type");
									meta.addInputParam(paramname->nodeValue(), paramtype->nodeValue());

#ifdef _DEBUG
	std::cout << "service has input param: " << paramname->nodeValue() << " " << paramtype->nodeValue() << std::endl;
#endif
								}
							}
						}
					}
					else if (childTag == "output")
					{
						NodeList *params = child->childNodes();
						if (params->length() != NULL)
						{
							for (unsigned int k = 0; k<params->length(); k++)
							{
								Node* param = params->item(k);
								if (param->localName() == "param")
								{
									NamedNodeMap* classAttribs = param->attributes();
									Node* paramname = classAttribs->getNamedItem("name");
									Node* paramtype = classAttribs->getNamedItem("type");
									meta.addInputParam(paramname->nodeValue(), paramtype->nodeValue());

#ifdef _DEBUG
	std::cout << "service has output param: " << paramname->nodeValue() << " " << paramtype->nodeValue() << std::endl;
#endif
								}
							}
						}
					}
					else if (!childTag.empty())
					{
						std::cout << "service's description contains unsupported element that will be ignored: " << childTag << std::endl;
					}
				}

				_info.addServiceMetadata(meta);
			}

		}
		catch (...)
		{

#ifdef _DEBUG
			std::cout << "metadata reader: error parsing doc" << std::endl;
#endif

			throw Exception::failure();

		}
	}

}