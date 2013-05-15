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

#include "xml/_2RealXMLWriter.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"
#include "app/_2RealEngine.h"
#include "app/_2RealBlockInfo.h"
#include "_2RealSystemState.h"

#include "Poco/TextEncoding.h"
#include "Poco/UTF8Encoding.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <list>

using std::string;
using std::ostringstream;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::vector;
using std::map;
using std::make_pair;
using std::list;

using namespace Poco::XML;

#define ROOT					"config"
#define BUNDLE					"bundle"
#define BUNDLE_LIST				"bundles"
#define BUNDLE_ID				"bundle_id"
#define BLOCK_ID				"block_id"
#define BLOCK_INSTANCE			"block_instance"
#define BLOCK_INSTANCE_LIST		"block_instances"
#define BLOCK_INSTANCE_ID		"block_instance_id"
#define LINK					"link"
#define LINK_LIST				"links"
#define INLET					"inlet"
#define BASIC_INLET				"basic_inlet"
#define BASIC_INLETS			"basic_inlets"
#define MULTI_INLET				"multi_inlet"
#define INLET_CONFIGURATION		"inlet_configuration"
#define INLET_CONFIGURATIONS	"inlet_configurations"
#define OUTLET					"outlet"
#define SETTINGS				"settings"
#define UPDATE_RATE				"fps"
#define INLET_UPDATE_POLICY		"policy"
#define INLET_BUFFER_SIZE		"buffer"
#define INLET_ID				"inlet_id"
#define INLET_BUFFER_VALUE		"value"
#define INLET_DEFAULT_VALUE		"default_value"
#define OUTLET_ID				"outlet_id"
#define BLOCK_STATE				"state"

namespace _2Real
{
		string Configuration::trimmedValue( string const& name, Poco::XML::Node &node )
		{
			Element *e = getChildElementByName( name, node );
			return trim( e->innerText() );
		}

		double Configuration::toDouble( string const& str )
		{
			stringstream sstr;
			double result;
			sstr << str;
			sstr >> result;
			return result;
		}

		float Configuration::toFloat( string const& str )
		{
			stringstream sstr;
			float result;
			sstr << str;
			sstr >> result;
			return result;
		}

		bool Configuration::toBool( string const& str )
		{
			stringstream sstr;
			bool result;
			sstr << str;
			sstr >> std::noboolalpha >> result;
			return result;
		}

		unsigned int Configuration::toUnsignedInt( string const& str )
		{
			stringstream sstr;
			unsigned int result;
			sstr << str;
			sstr >> result;
			std::cout << "to unsigned int: " << str << " " << result << std::endl;
			return result;
		}

		std::string Configuration::toString( const bool b )
		{
			stringstream sstr;
			sstr << std::noboolalpha << b;
			return sstr.str();
		}

		std::istream& operator>>( std::istream &in, Configuration &config )
		{
			//ifstream in( filePath );

			InputSource src( in );
			DOMParser parser;
			Document *document = parser.parse( &src );

			// find the framework - specific root; then get bundles/blocks/links
			// an exception might be thrown if the the child element does not exist
			Element *root		= Configuration::getChildElementByName( ROOT, *document );
			Element *bundles	= Configuration::getChildElementByName( BUNDLE_LIST, *root );
			Element *blocks		= Configuration::getChildElementByName( BLOCK_INSTANCE_LIST, *root );
			Element *links		= Configuration::getChildElementByName( LINK_LIST, *root );

			NodeList *installedBundles = bundles->childNodes();
			for ( unsigned int i=0; i<installedBundles->length(); ++i )
			{
				Node *bundle = installedBundles->item( i );
				if ( bundle->nodeType() == Node::ELEMENT_NODE && bundle->localName() == BUNDLE )
				{
					Configuration::BundleConfig bundleConfig;
					bundleConfig.bundleId = trim( bundle->innerText() );
					config.mBundleConfigurations.insert( std::make_pair( bundleConfig.bundleId, bundleConfig ) );
					std::cout << "needs bundle: " << bundleConfig.bundleId << std::endl;
				}
			}
			installedBundles->release();

			/**
			*	parse the instanced blocks
			**/
			NodeList *instancedBlocks = blocks->childNodes();
			for ( unsigned int i=0; i<instancedBlocks->length(); ++i )
			{
				Node *blockInstance = instancedBlocks->item( i );
				if ( blockInstance->nodeType() == Node::ELEMENT_NODE && blockInstance->localName() == BLOCK_INSTANCE )
				{
					//Element *identification = getChildElementByName( IDENTIFICATION, *blockInstance );
					Element *settings =Configuration::getChildElementByName( SETTINGS, *blockInstance );
					Element *inlets =Configuration::getChildElementByName( INLET_CONFIGURATIONS, *settings );

					Configuration::BlockConfig blockConfiguration;
					blockConfiguration.bundleId			= Configuration::trimmedValue( BUNDLE_ID, *blockInstance );
					blockConfiguration.blockId			= Configuration::trimmedValue( BLOCK_ID, *blockInstance );
					blockConfiguration.blockInstanceId	= Configuration::trimmedValue( BLOCK_INSTANCE_ID, *blockInstance );
					blockConfiguration.fps				= Configuration::trimmedValue( UPDATE_RATE, *settings );
					blockConfiguration.isRunning		= Configuration::trimmedValue( BLOCK_STATE, *settings );

					std::cout << "found block: " << blockConfiguration.blockInstanceId << std::endl;

					/**
					*	parse the block's inlets
					**/
					NodeList *inletConfigurations = inlets->childNodes();
					for ( unsigned int j=0; j<inletConfigurations->length(); ++j )
					{
						Node *inletConfiguration = inletConfigurations->item( j );
						if ( inletConfiguration->nodeType() == Node::ELEMENT_NODE && inletConfiguration->localName() == INLET_CONFIGURATION )
						{
							std::cout << "found inlet!" << std::endl;

							Configuration::InletConfig inletConfig;
							inletConfig.inletId = Configuration::trimmedValue( INLET_ID, *inletConfiguration );
							inletConfig.isMultiinlet = Configuration::trimmedValue( MULTI_INLET, *inletConfiguration );
							if (Configuration::toBool( inletConfig.isMultiinlet ) )
							{
								Element *basics =Configuration::getChildElementByName( BASIC_INLETS, *inletConfiguration );

								std::cout << "is a multiinlet" << std::endl;

								/**
								*	parse sub-inlets
								**/
								NodeList *basicInlets = basics->childNodes();
								for ( unsigned int u=0; u<basicInlets->length(); ++u )
								{
									Node *basicInlet = basicInlets->item( u );
									if ( basicInlet->nodeType() == Node::ELEMENT_NODE && basicInlet->localName() == BASIC_INLET )
									{
										Configuration::BasicInletConfig basicConfig;
										basicConfig.inletId			= Configuration::trimmedValue( INLET_ID, *basicInlet );
										basicConfig.updatePolicy	= Configuration::trimmedValue( INLET_UPDATE_POLICY, *basicInlet );
										basicConfig.bufferSize		= Configuration::trimmedValue( INLET_BUFFER_SIZE, *basicInlet );
										basicConfig.bufferedValue	= Configuration::trimmedValue( INLET_BUFFER_VALUE, *basicInlet );
										basicConfig.initialValue	= Configuration::trimmedValue( INLET_DEFAULT_VALUE, *basicInlet );
										inletConfig.basicInlets.push_back( basicConfig );
									}
								}
								basicInlets->release();
							}
							else
							{
								std::cout << "is a basic inlet" << std::endl;

								/*
								*	if it's a basic inlet, still store the extended info inside the sub inlets
								*/
								Configuration::BasicInletConfig basicConfig;
								basicConfig.inletId			= inletConfig.inletId;
								basicConfig.updatePolicy	= Configuration::trimmedValue( INLET_UPDATE_POLICY, *inletConfiguration );
								basicConfig.bufferSize		= Configuration::trimmedValue( INLET_BUFFER_SIZE, *inletConfiguration );
								basicConfig.bufferedValue	= Configuration::trimmedValue( INLET_BUFFER_VALUE, *inletConfiguration );
								basicConfig.initialValue	= Configuration::trimmedValue( INLET_DEFAULT_VALUE, *inletConfiguration );
								inletConfig.basicInlets.push_back( basicConfig );
							}

							// insert inlet w. uninitialized handle
							blockConfiguration.inlets.insert( std::make_pair( inletConfig.inletId, inletConfig ) );
						}
					}
					inletConfigurations->release();

					// insert block w. uninitialized handle
					config.mBlockConfigurations.insert( std::make_pair( blockConfiguration.blockInstanceId, blockConfiguration ) );
				}
			}
			instancedBlocks->release();

			/**
			*	parse links
			**/
			//std::multimap< ParamConfig, ParamConfig > linkConfigs;
			NodeList *createdLinks = links->childNodes();
			for ( unsigned int i=0; i<createdLinks->length(); ++i )
			{
				Node *link = createdLinks->item( i );
				if ( link->nodeType() == Node::ELEMENT_NODE && link->localName() == LINK )
				{
					Configuration::ParamConfig inletConfig;
					Configuration::ParamConfig outletConfig;

					Element *inlet = Configuration::getChildElementByName( INLET, *link );
					Element *outlet = Configuration::getChildElementByName( OUTLET, *link );

					inletConfig.paramId				= Configuration::trimmedValue( INLET_ID, *inlet );
					inletConfig.blockInstanceId		= Configuration::trimmedValue( BLOCK_INSTANCE_ID, *inlet );
					outletConfig.paramId			= Configuration::trimmedValue( OUTLET_ID, *outlet );
					outletConfig.blockInstanceId	= Configuration::trimmedValue( BLOCK_INSTANCE_ID, *outlet );

					map< string,Configuration::BlockConfig >::iterator inIter = config.mBlockConfigurations.find( inletConfig.blockInstanceId );
					map< string,Configuration::BlockConfig >::iterator outIter = config.mBlockConfigurations.find( outletConfig.blockInstanceId );
					if ( inIter == config.mBlockConfigurations.end() )
						throw NotFoundException( "" );
					if ( outIter == config.mBlockConfigurations.end() )
						throw NotFoundException( "" );
					config.mLinkConfigurations.insert( std::make_pair( inletConfig, outletConfig ) );

					std::cout << "found a link" << std::endl;
				}
			}
			createdLinks->release();

			return in;
		}

		Element * Configuration::getChildElementByName( string const& name, Node &node )
		{
			NodeList *children = node.childNodes();
			Element *child = nullptr;
			for ( unsigned int i=0; i<children->length(); ++i )
			{
				Node *n = children->item( i );
				if ( n->nodeType() == Node::ELEMENT_NODE && n->localName() == name )
				{
					if ( child != nullptr )
					{
						children->release();
						throw XMLFormatException( "xml node \'" + node.nodeName() + "\' has multiple children named \'" + name + "\'" );
					}

					child = static_cast< Element * >( n );
				}
			}

			if ( child == nullptr )
			{
				children->release();
				throw XMLFormatException( "xml node \'" + node.nodeName() + "\' has no child named \'" + name + "\'" );
			}

			children->release();
			return child;
		}

		Configuration::Configuration() :
			mDocument( new Document() ),
			mRoot( mDocument->createElement( ROOT ) ),
			mBundles( mDocument->createElement( BUNDLE_LIST ) ),
			mBlocks( mDocument->createElement( BLOCK_INSTANCE_LIST ) ),
			mLinks( mDocument->createElement( LINK_LIST ) )
		{
			mDocument->appendChild( mRoot );
			mRoot->appendChild( mBundles );
			mRoot->appendChild( mBlocks );
			mRoot->appendChild( mLinks );
		}

		//voidConfiguration::writeTo( string const& filePath ) const
		//{
		//	ofstream file( filePath );

		//	DOMWriter writer;
		//	writer.setNewLine("\n");
		//	writer.setOptions( XMLWriter::PRETTY_PRINT );
		//	writer.writeNode( file, m_Document );
		//}

		//voidConfiguration::addBundle( BundleConfig const& config )
		//{
		//	AutoPtr< Element > bundle = m_Document->createElement( BUNDLE );
		//	m_Bundles->appendChild( bundle );
		//	AutoPtr< Text > path = m_Document->createTextNode( config.bundleName );
		//	bundle->appendChild( path );
		//}

		//voidConfiguration::addBlockInstance( BlockConfig const& config )
		//{
		//	AutoPtr< Element > blockInstance = m_Document->createElement( BLOCK_INSTANCE );
		//	AutoPtr< Element > bundle = m_Document->createElement( BUNDLE_ID );
		//	AutoPtr< Element > block = m_Document->createElement( BLOCK_ID );
		//	AutoPtr< Element > name = m_Document->createElement( BLOCK_INSTANCE_ID );
		//	AutoPtr< Text > bundlePath = m_Document->createTextNode( config.bundleId );
		//	AutoPtr< Text > blockName = m_Document->createTextNode( config.blockId );
		//	AutoPtr< Text > instanceName = m_Document->createTextNode( config.blockInstanceId );
		//	m_Blocks->appendChild( blockInstance );
		//	blockInstance->appendChild( bundle );
		//	bundle->appendChild( bundlePath );
		//	blockInstance->appendChild( block );
		//	block->appendChild( blockName );
		//	blockInstance->appendChild( name );
		//	name->appendChild( instanceName );

		//	AutoPtr< Element > settings = m_Document->createElement( SETTINGS );
		//	blockInstance->appendChild( settings );
		//	AutoPtr< Element > updates = m_Document->createElement( UPDATE_RATE );
		//	AutoPtr< Text > fps = m_Document->createTextNode( config.fps );
		//	AutoPtr< Element > blockState = m_Document->createElement( BLOCK_STATE );
		//	AutoPtr< Text > state = m_Document->createTextNode( config.isRunning ? "1" : "0" );
		//	settings->appendChild( updates );
		//	updates->appendChild( fps );
		//	settings->appendChild( blockState );
		//	blockState->appendChild( state );
		//	AutoPtr< Element > inlets = m_Document->createElement( INLET_CONFIGURATIONS );
		//	settings->appendChild( inlets );

		//	for ( vector< InletConfig >::const_iterator it = config.inlets.begin(); it != config.inlets.end(); ++it )
		//	{
		//		AutoPtr< Element > inlet = m_Document->createElement( INLET_CONFIGURATION );
		//		inlets->appendChild( inlet );

		//		AutoPtr< Element > inletName = m_Document->createElement( INLET_ID );
		//		AutoPtr< Text > name = m_Document->createTextNode( it->inletId );
		//		inlet->appendChild( inletName );
		//		inletName->appendChild( name );

		//		AutoPtr< Element > isMulti = m_Document->createElement( MULTI_INLET );
		//		inlet->appendChild( isMulti );

		//		if ( it->isMulti )
		//		{
		//			AutoPtr< Text > multiValue = m_Document->createTextNode( "1" );
		//			isMulti->appendChild( multiValue );

		//			AutoPtr< Element > basicInlets = m_Document->createElement( BASIC_INLETS );
		//			inlet->appendChild( basicInlets );

		//			for ( vector< BasicInletConfig >::const_iterator bIt = it->basicInlets.begin(); bIt != it->basicInlets.end(); ++bIt )
		//			{
		//				AutoPtr< Element > basicInlet = m_Document->createElement( BASIC_INLET );
		//				basicInlets->appendChild( basicInlet );

		//				AutoPtr< Element > inId = m_Document->createElement( INLET_ID );
		//				AutoPtr< Element > inletPolicy = m_Document->createElement( INLET_UPDATE_POLICY );
		//				AutoPtr< Element > inletBuffer = m_Document->createElement( INLET_BUFFER_SIZE );
		//				AutoPtr< Element > inletBufferValue = m_Document->createElement( INLET_BUFFER_VALUE );
		//				AutoPtr< Element > inletDefaultValue = m_Document->createElement( INLET_DEFAULT_VALUE );
		//				AutoPtr< Text > id = m_Document->createTextNode( bIt->inletId );
		//				AutoPtr< Text > policy = m_Document->createTextNode( bIt->updatePolicy );
		//				AutoPtr< Text > buffer = m_Document->createTextNode( bIt->bufferSize );
		//				AutoPtr< Text > bufferVal = m_Document->createTextNode( bIt->bufferedValue );
		//				AutoPtr< Text > defaultVal = m_Document->createTextNode( bIt->initialValue );
		//				basicInlet->appendChild( inId );
		//				inId->appendChild( id );
		//				basicInlet->appendChild( inletPolicy );
		//				inletPolicy->appendChild( policy );
		//				basicInlet->appendChild( inletBuffer );
		//				inletBuffer->appendChild( buffer );
		//				basicInlet->appendChild( inletBufferValue );
		//				inletBufferValue->appendChild( bufferVal );
		//				basicInlet->appendChild( inletDefaultValue );
		//				inletDefaultValue->appendChild( defaultVal );
		//			}
		//		}
		//		else
		//		{
		//			AutoPtr< Text > multiValue = m_Document->createTextNode( "0" );
		//			isMulti->appendChild( multiValue );

		//			BasicInletConfig c = it->basicInlets[ 0 ];
		//			AutoPtr< Element > inletPolicy = m_Document->createElement( INLET_UPDATE_POLICY );
		//			AutoPtr< Element > inletBuffer = m_Document->createElement( INLET_BUFFER_SIZE );
		//			AutoPtr< Element > inletBufferValue = m_Document->createElement( INLET_BUFFER_VALUE );
		//			AutoPtr< Element > inletDefaultValue = m_Document->createElement( INLET_DEFAULT_VALUE );
		//			AutoPtr< Text > policy = m_Document->createTextNode( c.updatePolicy );
		//			AutoPtr< Text > buffer = m_Document->createTextNode( c.bufferSize );
		//			AutoPtr< Text > bufferVal = m_Document->createTextNode( c.bufferedValue );
		//			AutoPtr< Text > defaultVal = m_Document->createTextNode( c.initialValue );
		//			inlet->appendChild( inletPolicy );
		//			inletPolicy->appendChild( policy );
		//			inlet->appendChild( inletBuffer );
		//			inletBuffer->appendChild( buffer );
		//			inlet->appendChild( inletBufferValue );
		//			inletBufferValue->appendChild( bufferVal );
		//			inlet->appendChild( inletDefaultValue );
		//			inletDefaultValue->appendChild( defaultVal );
		//		}
		//	}
		//}

		//voidConfiguration::addLink( ParamConfig const& inletConfig, ParamConfig const& outletConfig )
		//{
		//	AutoPtr< Element > link = m_Document->createElement( LINK );
		//	AutoPtr< Element > inlet = m_Document->createElement( INLET );
		//	AutoPtr< Element > inletName = m_Document->createElement( INLET_ID );
		//	AutoPtr< Element > inletBlock = m_Document->createElement( BLOCK_INSTANCE_ID );
		//	AutoPtr< Element > outlet = m_Document->createElement( OUTLET );
		//	AutoPtr< Element > outletName = m_Document->createElement( OUTLET_ID );
		//	AutoPtr< Element > outletBlock = m_Document->createElement( BLOCK_INSTANCE_ID );
		//	m_Links->appendChild( link );
		//	link->appendChild( inlet );
		//	link->appendChild( outlet );
		//	inlet->appendChild( inletName );
		//	inlet->appendChild( inletBlock );
		//	outlet->appendChild( outletName );
		//	outlet->appendChild( outletBlock);
		//	inletName->appendChild( m_Document->createTextNode( inletConfig.paramId ) );
		//	inletBlock->appendChild( m_Document->createTextNode( inletConfig.blockInstanceId ) );
		//	outletName->appendChild( m_Document->createTextNode( outletConfig.paramId ) );
		//	outletBlock->appendChild( m_Document->createTextNode( outletConfig.blockInstanceId ) );
		//}
}