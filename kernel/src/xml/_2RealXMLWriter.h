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
		class BlockHandle;
	};

	class Bundle;
	template< typename T >
	class FunctionBlock;
	class AbstractInletIO;
	class BasicInletIO;

	class Configuration
	{

	public:

		struct BundleConfig
		{
			std::string								bundleId;
			Bundle									*bundle;
		};

		struct BasicInletConfig
		{
			std::string								inletId;
			std::string								bufferSize;
			std::string								updatePolicy;
			std::string								initialValue;
			std::string								bufferedValue;
			BasicInletIO							*inlet;
		};

		struct InletConfig
		{
			std::string								inletId;
			std::string								isMultiinlet;
			std::vector< BasicInletConfig >			basicInlets;
			AbstractInletIO							*inlet;
		};

		struct BlockConfig
		{
			std::string								bundleId;
			std::string								blockId;
			std::string								blockInstanceId;
			std::string								fps;
			std::string								isRunning;
			std::map< std::string, InletConfig >	inlets;
			FunctionBlock< app::BlockHandle >		*block;
		};

		struct ParamConfig
		{
			std::string					blockInstanceId;
			std::string					paramId;

			bool operator<( ParamConfig const& other ) const
			{
				if ( blockInstanceId < other.blockInstanceId ) return true;
				if ( other.blockInstanceId < blockInstanceId ) return false;
				if ( paramId < other.paramId ) return true;
				return false;
			}
		};

		typedef std::map< std::string, BundleConfig >		BundleConfigurations;
		typedef std::map< std::string, BlockConfig >		BlockConfigurations;
		typedef std::multimap< ParamConfig, ParamConfig >	LinkConfigurations;

		Configuration();

		BundleConfigurations &bundleConfigs() { return mBundleConfigurations; }
		BlockConfigurations &blockConfigs() { return mBlockConfigurations; }
		LinkConfigurations &linkConfigs() { return mLinkConfigurations; }

		//void addBundle( BundleConfig const& bundle );
		//void addBlockInstance( BlockConfig const& block );
		//void addLink( ParamConfig const& inlet, ParamConfig const& outlet );
		//void writeTo( std::string const& filePath ) const;
		//static void loadConfig( app::Engine &engine, std::string const& filePath );
		//static std::list< std::string > tryConfig( app::Engine &engine, std::string const& filePath );
		//static app::SystemState * loadConfiguration( std::string const& dataSource, std::map< std::string, app::BundleHandle > &bundles, const bool startAll = true );

		static std::string trimmedValue( std::string const& name, Poco::XML::Node &node );

		static double toDouble( std::string const& str );
		static float toFloat( std::string const& str );
		static unsigned int toUnsignedInt( std::string const& str );
		static bool toBool( std::string const& str );

		static std::string toString( const bool b );
		static std::string toString( const unsigned int u );
		static std::string toString( const float f );
		static std::string toString( const double d );

		friend std::ostream& operator<<( std::ostream &out, Configuration const& config );
		friend std::istream& operator>>( std::istream &in, Configuration &config );

	private:

		static Poco::XML::Element * getChildElementByName( std::string const& name, Poco::XML::Node &node );

		BlockConfigurations						mBlockConfigurations;
		LinkConfigurations						mLinkConfigurations;
		BundleConfigurations					mBundleConfigurations;

		Poco::AutoPtr< Poco::XML::Document >	mDocument;
		Poco::AutoPtr< Poco::XML::Element >		mRoot;
		Poco::AutoPtr< Poco::XML::Element >		mBundles;
		Poco::AutoPtr< Poco::XML::Element >		mBlocks;
		Poco::AutoPtr< Poco::XML::Element >		mLinks;

	};
}