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

#include "Common.h"

#include "Poco/SAX/ContentHandler.h"


/*
	responsible for reading & storing plugin metadata
*/

namespace _2Real
{
	class PluginManifest : public Poco::XML::ContentHandler
	{
	public:
		
		PluginManifest(std::string path);
		~PluginManifest();

		void readPluginManifest();												//read the xml file
		const std::string getAttribute(const std::string& key) const;

	private:

		std::string				m_ManifestFile;
		PluginMetadata			m_Attributes;

		void startDocument();
		void endDocument();
		void startElement(const Poco::XML::XMLString& namespaceURI, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes);
		void endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname);
		
		void characters(const Poco::XML::XMLChar ch[], int start, int length)								{}
		void ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int len)							{}
		void processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data)	{}
		void startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri)		{}
		void endPrefixMapping(const Poco::XML::XMLString& prefix)											{}
		void skippedEntity(const Poco::XML::XMLString& name)												{}
		void setDocumentLocator(const Poco::XML::Locator*)													{}
	};
}
