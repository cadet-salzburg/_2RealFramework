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

#include "xml/_2RealXML.h"
#include "helpers/_2RealPoco.h"

#include <string>
#include <vector>

namespace _2Real
{
	namespace app
	{
		class Engine;
	}

	namespace xml
	{
		struct BundleConfig
		{
			std::string						bundleName;
			std::string						bundlePath;
		};

		struct BasicInletConfig
		{
			std::string						inletId;
			std::string						bufferSize;
			std::string						updatePolicy;
			std::string						initialValue;
			std::string						bufferedValue;
		};

		struct InletConfig
		{
			std::string						inletId;
			bool							isMulti;
			std::vector< BasicInletConfig >	basicInlets;
		};

		struct BlockConfig
		{
			std::string					bundleId;
			std::string					blockId;
			std::string					blockInstanceId;
			std::string					fps;
			std::vector< InletConfig >	inlets;
			bool						isRunning;
		};

		struct ParamConfig
		{
			std::string					blockInstanceId;
			std::string					paramId;
		};

		struct InletHandleId
		{
			std::string					blockInstanceId;
			std::string					inletId;

			bool operator<( InletHandleId const& other ) const
			{
				if ( blockInstanceId < other.blockInstanceId ) return true;
				if ( other.blockInstanceId < blockInstanceId ) return false;
				if ( inletId < other.inletId ) return true;
				return false;
			}
		};

		class XMLConfig
		{

		public:

			XMLConfig();
			void addBundle( BundleConfig const& bundle );
			void addBlockInstance( BlockConfig const& block );
			void addLink( ParamConfig const& inlet, ParamConfig const& outlet );
			void writeTo( std::string const& filePath ) const;

			static void loadConfig( app::Engine &engine, std::string const& filePath );
			static std::list< std::string > tryConfig( app::Engine &engine, std::string const& filePath );

		private:

			static Poco::XML::Element * getChildElementByName( std::string const& name, Poco::XML::Node &node );

			Poco::AutoPtr< Poco::XML::Document >	m_Document;
			Poco::AutoPtr< Poco::XML::Element >		m_Root;
			Poco::AutoPtr< Poco::XML::Element >		m_Bundles;
			Poco::AutoPtr< Poco::XML::Element >		m_Blocks;
			Poco::AutoPtr< Poco::XML::Element >		m_Links;

		};
	}
}