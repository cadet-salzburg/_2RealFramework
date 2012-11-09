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
#include "app/_2RealBlockHandle.h"
#include "app/_2RealBlockInfo.h"
#include "app/_2RealBundleHandle.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"

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
	namespace xml
	{
		list< string > XMLConfig::tryConfig( app::Engine &engine, string const& filePath )
		{
			ifstream in( filePath );
			InputSource src( in );
			DOMParser parser;

			Document *document = parser.parse( &src );
			Element *root = getChildElementByName( ROOT, *document );
			Element *bundles = getChildElementByName( BUNDLE_LIST, *root );

			list< string > neededBundles;
			NodeList *bundleList = bundles->childNodes();
			for ( unsigned int i=0; i<bundleList->length(); ++i )
			{
				Node *bundle = bundleList->item( i );
				if ( bundle->nodeType() == Node::ELEMENT_NODE && bundle->localName() == BUNDLE )
				{
					string name = trim( bundle->innerText() );

					try
					{
						app::BundleHandle handle = engine.findBundleByName( name );
					}
					catch ( NotFoundException &e )
					{
						( void ) e;
						neededBundles.push_back( name );
					}
				}
			}
			bundleList->release();
			return neededBundles;
		}

		void XMLConfig::loadConfig( app::Engine &engine, string const& filePath )
		{
			ifstream in( filePath );
			InputSource src( in );
			DOMParser parser;

			Document *document = parser.parse( &src );
			Element *root = getChildElementByName( ROOT, *document );
			Element *bundles = getChildElementByName( BUNDLE_LIST, *root );
			Element *blocks = getChildElementByName( BLOCK_INSTANCE_LIST, *root );
			Element *links = getChildElementByName( LINK_LIST, *root );

			/**
			*	find the installed bundles
			**/

			map< string, app::BundleHandle > loadedBundles;
			NodeList *installedBundles = bundles->childNodes();
			for ( unsigned int i=0; i<installedBundles->length(); ++i )
			{
				Node *bundle = installedBundles->item( i );
				if ( bundle->nodeType() == Node::ELEMENT_NODE && bundle->localName() == BUNDLE )
				{
					BundleConfig config;
					config.bundleName = trim( bundle->innerText() );

					app::BundleHandle handle = engine.findBundleByName( config.bundleName );
					loadedBundles.insert( make_pair( config.bundleName, handle ) );
				}
			}
			installedBundles->release();

			map< string, app::BlockHandle > blockInstances;					// holds a list of block instances & names in previous file
			map< string, bool > blockStates;								// holds the state ( started or not started )
			map< InletHandleId, app::InletHandle > inletHandles;
			map< app::BlockHandle, vector< InletConfig > > inletConfigs;

			NodeList *instancedBlocks = blocks->childNodes();
			for ( unsigned int i=0; i<instancedBlocks->length(); ++i )
			{
				Node *blockInstance = instancedBlocks->item( i );
				if ( blockInstance->nodeType() == Node::ELEMENT_NODE && blockInstance->localName() == BLOCK_INSTANCE )
				{
					BlockConfig config;
					Element *bundleId = getChildElementByName( BUNDLE_ID, *blockInstance );
					config.bundleId = trim( bundleId->innerText() );
					Element *blockId = getChildElementByName( BLOCK_ID, *blockInstance );
					config.blockId = trim( blockId->innerText() );
					Element *blockInstanceId = getChildElementByName( BLOCK_INSTANCE_ID, *blockInstance );
					config.blockInstanceId = trim( blockInstanceId->innerText() );

					app::BlockHandle blockHandle;
					map< string, app::BundleHandle >::iterator it = loadedBundles.find( config.bundleId );
					if ( it != loadedBundles.end() && it->second.isValid() )
					{
						blockHandle = it->second.createBlockInstance( config.blockId );
						blockInstances.insert( make_pair( config.blockInstanceId, blockHandle ) );
					}

					Element *settings = getChildElementByName( SETTINGS, *blockInstance );
					Element *fps = getChildElementByName( UPDATE_RATE, *settings );
					config.fps = trim( fps->innerText() );

					Element *state = getChildElementByName( BLOCK_STATE, *settings );
					config.isRunning = ( state->innerText() == "1" );

					blockStates.insert( make_pair( config.blockInstanceId, config.isRunning ) );

					stringstream sFps;
					double updateRate;
					sFps << config.fps;
					sFps >> updateRate;

					//blockHandle.setUpdateRate( updateRate );

					Element *inlets = getChildElementByName( INLET_CONFIGURATIONS, *settings );
					NodeList *inletConfigurations = inlets->childNodes();
					for ( unsigned int j=0; j<inletConfigurations->length(); ++j )
					{
						Node *inletConfiguration = inletConfigurations->item( j );
						if ( inletConfiguration->nodeType() == Node::ELEMENT_NODE && inletConfiguration->localName() == INLET_CONFIGURATION )
						{
							InletConfig inConfig;
							Element *inletId = getChildElementByName( INLET_ID, *inletConfiguration );
							inConfig.inletId = trim( inletId->innerText() );
							Element *isMultiInlet = getChildElementByName( MULTI_INLET, *inletConfiguration );
							inConfig.isMulti = ( isMultiInlet->innerText() == "1" );

							if ( inConfig.isMulti )
							{
								app::InletHandle inletHandle = blockHandle.getInletHandle( inConfig.inletId );

								Element *basics = getChildElementByName( BASIC_INLETS, *inletConfiguration );
								NodeList *basicInlets = basics->childNodes();
								for ( unsigned int u=0; u<basicInlets->length(); ++u )
								{
									Node *basicInlet = basicInlets->item( u );
									if ( basicInlet->nodeType() == Node::ELEMENT_NODE && basicInlet->localName() == BASIC_INLET )
									{
										BasicInletConfig basicConfig;

										Element *basicName = getChildElementByName( INLET_ID, *basicInlet );
										basicConfig.inletId = trim( basicName->innerText() );
										Element *updatePolicy = getChildElementByName( INLET_UPDATE_POLICY, *basicInlet );
										basicConfig.updatePolicy = trim( updatePolicy->innerText() );
										Element *bufferSize = getChildElementByName( INLET_BUFFER_SIZE, *basicInlet );
										basicConfig.bufferSize = trim( bufferSize->innerText() );
										Element *value = getChildElementByName( INLET_BUFFER_VALUE, *basicInlet );
										basicConfig.bufferedValue = trim( value->innerText() );
										Element *initialValue = getChildElementByName( INLET_DEFAULT_VALUE, *basicInlet );
										basicConfig.initialValue = trim( initialValue->innerText() );
										inConfig.basicInlets.push_back( basicConfig );

										app::InletHandle subHandle;
										subHandle = inletHandle.add();

										//if ( !basicConfig.initialValue.empty() )
										//{
										//	if ( inletHandle.getName() == "string" )
										//	{
										//		std::cout << "found string default value for " << subHandle.getName() << std::endl;
										//		subHandle.setDefaultValue< string >( basicConfig.initialValue );
										//	}
										//	else if ( basicConfig.initialValue.find( "UNSTREAMABLE" ) != string::npos )
										//	{
										//		std::cout << "found unreadable default value for " << subHandle.getName() << std::endl;
										//	}
										//	else
										//	{
										//		std::cout << "found default value for " << subHandle.getName() << " " << basicConfig.initialValue << std::endl;
										//		subHandle.setDefaultValueToString( basicConfig.initialValue );
										//	}
										//}

										stringstream sBuffer;
										unsigned int buffer;
										sBuffer << basicConfig.bufferSize;
										sBuffer >> buffer;

										subHandle.setBufferSize( buffer );

										InletPolicy p = InletPolicy::getPolicyFromString( basicConfig.updatePolicy );
										subHandle.setUpdatePolicy( p );

										InletHandleId id;
										id.blockInstanceId = config.blockInstanceId;
										id.inletId = basicConfig.inletId;
										inletHandles.insert( make_pair( id, subHandle ) );
									}
								}
								basicInlets->release();
							}
							else
							{
								BasicInletConfig basicConfig;
								basicConfig.inletId = inConfig.inletId;
								Element *updatePolicy = getChildElementByName( INLET_UPDATE_POLICY, *inletConfiguration );
								basicConfig.updatePolicy = trim( updatePolicy->innerText() );
								Element *bufferSize = getChildElementByName( INLET_BUFFER_SIZE, *inletConfiguration );
								basicConfig.bufferSize = trim( bufferSize->innerText() );
								Element *value = getChildElementByName( INLET_BUFFER_VALUE, *inletConfiguration );
								basicConfig.bufferedValue = trim( value->innerText() );
								Element *initialValue = getChildElementByName( INLET_DEFAULT_VALUE, *inletConfiguration );
								basicConfig.initialValue = trim( initialValue->innerText() );
								inConfig.basicInlets.push_back( basicConfig );

								app::InletHandle inletHandle = blockHandle.getInletHandle( basicConfig.inletId );
								//if ( !basicConfig.initialValue.empty() )
								//{
								//	if ( inletHandle.getName() == "string" )
								//	{
								//		std::cout << "found string default value for " << inletHandle.getName() << std::endl;
								//		inletHandle.setDefaultValue< string >( basicConfig.initialValue );
								//	}
								//	else if ( basicConfig.initialValue.find( "UNSTREAMABLE" ) != string::npos )
								//	{
								//		std::cout << "found unreadable default value for " << inletHandle.getName() << std::endl;
								//	}
								//	else
								//	{
								//		std::cout << "found default value for " << inletHandle.getName() << " " << basicConfig.initialValue << std::endl;
								//		inletHandle.setDefaultValueToString( basicConfig.initialValue );
								//	}
								//}

								stringstream sBuffer;
								unsigned int buffer;
								sBuffer << basicConfig.bufferSize;
								sBuffer >> buffer;

								inletHandle.setBufferSize( buffer );
								InletPolicy p = InletPolicy::getPolicyFromString( basicConfig.updatePolicy );
								inletHandle.setUpdatePolicy( p );

								InletHandleId id;
								id.blockInstanceId = config.blockInstanceId;
								id.inletId = basicConfig.inletId;
								inletHandles.insert( make_pair( id, inletHandle ) );
							}

							inletConfigs[ blockHandle ].push_back( inConfig );
						}
					}
					inletConfigurations->release();
				}
			}
			instancedBlocks->release();

			NodeList *createdLinks = links->childNodes();
			for ( unsigned int i=0; i<createdLinks->length(); ++i )
			{
				Node *link = createdLinks->item( i );
				if ( link->nodeType() == Node::ELEMENT_NODE && link->localName() == LINK )
				{
					ParamConfig inletConfig;
					ParamConfig outletConfig;

					Element *inlet = getChildElementByName( INLET, *link );
					Element *inletId = getChildElementByName( INLET_ID, *inlet );
					inletConfig.paramId = trim( inletId->innerText() );
					Element *inletblockInstanceId = getChildElementByName( BLOCK_INSTANCE_ID, *inlet );
					inletConfig.blockInstanceId = trim( inletblockInstanceId->innerText() );
					Element *outlet = getChildElementByName( OUTLET, *link );
					Element *outletId = getChildElementByName( OUTLET_ID, *outlet );
					outletConfig.paramId = trim( outletId->innerText() );
					Element *outletblockInstanceId = getChildElementByName( BLOCK_INSTANCE_ID, *outlet );
					outletConfig.blockInstanceId = trim( outletblockInstanceId->innerText() );

					map< string, app::BlockHandle >::iterator inIt = blockInstances.find( inletConfig.blockInstanceId );
					map< string, app::BlockHandle >::iterator outIt = blockInstances.find( outletConfig.blockInstanceId );

					if ( inIt == blockInstances.end() )
					{
						std::cout << "missing " << inletConfig.blockInstanceId << std::endl;
					}

					if ( outIt == blockInstances.end() )
					{
						std::cout << "missing " << outletConfig.blockInstanceId << std::endl;
					}

					if ( inIt != blockInstances.end() && outIt != blockInstances.end() && inIt->second.isValid() && outIt->second.isValid() )
					{
						InletHandleId id;
						id.blockInstanceId = inletConfig.blockInstanceId;
						id.inletId = inletConfig.paramId;

						map< InletHandleId, app::InletHandle >::iterator inHandleIt = inletHandles.find( id );

						if ( inHandleIt == inletHandles.end() )
						{
							std::cout << "notfound " << inletConfig.paramId << std::endl;
							continue;
						}

						app::InletHandle inHandle = inHandleIt->second;
						app::OutletHandle outHandle = outIt->second.getOutletHandle( outletConfig.paramId );

						if ( !inHandle.link( outHandle ) )
						{
							inHandle.linkWithConversion( outHandle );
						}
					}
				}
			}
			createdLinks->release();

			for ( map< string, app::BlockHandle >::iterator it = blockInstances.begin(); it != blockInstances.end(); ++it )
			{
				it->second.setup();
				std::cout << "set up block: " << it->first << std::endl;
			}

			for ( map< string, app::BlockHandle >::iterator it = blockInstances.begin(); it != blockInstances.end(); ++it )
			{
				map< app::BlockHandle, vector< InletConfig > >::iterator iIt = inletConfigs.find( it->second );
				if ( iIt != inletConfigs.end() )
				{
					for ( vector< InletConfig >::iterator cIt = iIt->second.begin(); cIt != iIt->second.end(); ++cIt )
					{
						app::InletHandle handle = it->second.getInletHandle( cIt->inletId );
						vector< BasicInletConfig > subInlets = cIt->basicInlets;

						if ( cIt->isMulti )
						{
							unsigned int i=0;
							for ( vector< BasicInletConfig >::iterator bIt = subInlets.begin(); bIt != subInlets.end(); ++bIt )
							{
								if ( !bIt->bufferedValue.empty() )
								{
									if ( handle.getTypename() == "string" )
									{
										std::cout << "setting value of " << handle.getName() << " to " << bIt->bufferedValue << std::endl;
										handle[ i ].setValue< string >( bIt->bufferedValue );
									}
									else if ( handle.getTypename().find( "vector" ) != std::string::npos )
									{
										//std::cout << "setting value of " << handle.getName() << " to " << config.bufferedValue << std::endl;
										handle[ i ].setValueToString( bIt->bufferedValue );
									}
									else if ( bIt->bufferedValue.find( "UNSTREAMABLE" ) != string::npos )
									{
										std::cout << "found unreadable buffered value for " << handle.getName() << std::endl;
									}
									else
									{
										std::cout << "setting value of " << handle.getName() << " to " << bIt->bufferedValue << std::endl;
										handle[ i ].setValueToString( bIt->bufferedValue );
									}
								}
								++i;
							}
						}
						else
						{
							BasicInletConfig config = subInlets.front();
							if ( !config.bufferedValue.empty() )
							{
								if ( handle.getName() == "string" )
								{
									std::cout << "setting value of " << handle.getName() << " to " << config.bufferedValue << std::endl;
									handle.setValue< string >( config.bufferedValue );
								}
								else if ( handle.getTypename().find( "vector" ) != std::string::npos )
								{
									//std::cout << "setting value of " << handle.getName() << " to " << config.bufferedValue << std::endl;
									handle.setValueToString( config.bufferedValue );
								}
								else if ( config.bufferedValue.find( "UNSTREAMABLE" ) != string::npos )
								{
									std::cout << "found unreadable buffered value for " << handle.getName() << std::endl;
								}
								else
								{
									std::cout << "setting value of " << handle.getName() << " to " << config.bufferedValue << std::endl;
									handle.setValueToString( config.bufferedValue );
								}
							}
						}
					}
				}
			}

			for ( map< string, app::BlockHandle >::iterator it = blockInstances.begin(); it != blockInstances.end(); ++it )
			{
				if ( blockStates[ it->first ] )
				{
					it->second.start();
					std::cout << "started block: " << it->first << std::endl;
				}
			}
		}

		Element * XMLConfig::getChildElementByName( string const& name, Node &node )
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

		XMLConfig::XMLConfig() :
			m_Document( new Document() ),
			m_Root( m_Document->createElement( ROOT ) ),
			m_Bundles( m_Document->createElement( BUNDLE_LIST ) ),
			m_Blocks( m_Document->createElement( BLOCK_INSTANCE_LIST ) ),
			m_Links( m_Document->createElement( LINK_LIST ) )
		{
			m_Document->appendChild( m_Root );
			m_Root->appendChild( m_Bundles );
			m_Root->appendChild( m_Blocks );
			m_Root->appendChild( m_Links );
		}

		void XMLConfig::writeTo( string const& filePath ) const
		{
			ofstream file( filePath );

			DOMWriter writer;
			writer.setNewLine("\n");
			writer.setOptions( XMLWriter::PRETTY_PRINT );
			writer.writeNode( file, m_Document );
		}

		void XMLConfig::addBundle( BundleConfig const& config )
		{
			AutoPtr< Element > bundle = m_Document->createElement( BUNDLE );
			m_Bundles->appendChild( bundle );
			AutoPtr< Text > path = m_Document->createTextNode( config.bundleName );
			bundle->appendChild( path );
		}

		void XMLConfig::addBlockInstance( BlockConfig const& config )
		{
			AutoPtr< Element > blockInstance = m_Document->createElement( BLOCK_INSTANCE );
			AutoPtr< Element > bundle = m_Document->createElement( BUNDLE_ID );
			AutoPtr< Element > block = m_Document->createElement( BLOCK_ID );
			AutoPtr< Element > name = m_Document->createElement( BLOCK_INSTANCE_ID );
			AutoPtr< Text > bundlePath = m_Document->createTextNode( config.bundleId );
			AutoPtr< Text > blockName = m_Document->createTextNode( config.blockId );
			AutoPtr< Text > instanceName = m_Document->createTextNode( config.blockInstanceId );
			m_Blocks->appendChild( blockInstance );
			blockInstance->appendChild( bundle );
			bundle->appendChild( bundlePath );
			blockInstance->appendChild( block );
			block->appendChild( blockName );
			blockInstance->appendChild( name );
			name->appendChild( instanceName );

			AutoPtr< Element > settings = m_Document->createElement( SETTINGS );
			blockInstance->appendChild( settings );
			AutoPtr< Element > updates = m_Document->createElement( UPDATE_RATE );
			AutoPtr< Text > fps = m_Document->createTextNode( config.fps );
			AutoPtr< Element > blockState = m_Document->createElement( BLOCK_STATE );
			AutoPtr< Text > state = m_Document->createTextNode( config.isRunning ? "1" : "0" );
			settings->appendChild( updates );
			updates->appendChild( fps );
			settings->appendChild( blockState );
			blockState->appendChild( state );
			AutoPtr< Element > inlets = m_Document->createElement( INLET_CONFIGURATIONS );
			settings->appendChild( inlets );

			for ( vector< InletConfig >::const_iterator it = config.inlets.begin(); it != config.inlets.end(); ++it )
			{
				AutoPtr< Element > inlet = m_Document->createElement( INLET_CONFIGURATION );
				inlets->appendChild( inlet );

				AutoPtr< Element > inletName = m_Document->createElement( INLET_ID );
				AutoPtr< Text > name = m_Document->createTextNode( it->inletId );
				inlet->appendChild( inletName );
				inletName->appendChild( name );

				AutoPtr< Element > isMulti = m_Document->createElement( MULTI_INLET );
				inlet->appendChild( isMulti );

				if ( it->isMulti )
				{
					AutoPtr< Text > multiValue = m_Document->createTextNode( "1" );
					isMulti->appendChild( multiValue );

					AutoPtr< Element > basicInlets = m_Document->createElement( BASIC_INLETS );
					inlet->appendChild( basicInlets );

					for ( vector< BasicInletConfig >::const_iterator bIt = it->basicInlets.begin(); bIt != it->basicInlets.end(); ++bIt )
					{
						AutoPtr< Element > basicInlet = m_Document->createElement( BASIC_INLET );
						basicInlets->appendChild( basicInlet );

						AutoPtr< Element > inId = m_Document->createElement( INLET_ID );
						AutoPtr< Element > inletPolicy = m_Document->createElement( INLET_UPDATE_POLICY );
						AutoPtr< Element > inletBuffer = m_Document->createElement( INLET_BUFFER_SIZE );
						AutoPtr< Element > inletBufferValue = m_Document->createElement( INLET_BUFFER_VALUE );
						AutoPtr< Element > inletDefaultValue = m_Document->createElement( INLET_DEFAULT_VALUE );
						AutoPtr< Text > id = m_Document->createTextNode( bIt->inletId );
						AutoPtr< Text > policy = m_Document->createTextNode( bIt->updatePolicy );
						AutoPtr< Text > buffer = m_Document->createTextNode( bIt->bufferSize );
						AutoPtr< Text > bufferVal = m_Document->createTextNode( bIt->bufferedValue );
						AutoPtr< Text > defaultVal = m_Document->createTextNode( bIt->initialValue );
						basicInlet->appendChild( inId );
						inId->appendChild( id );
						basicInlet->appendChild( inletPolicy );
						inletPolicy->appendChild( policy );
						basicInlet->appendChild( inletBuffer );
						inletBuffer->appendChild( buffer );
						basicInlet->appendChild( inletBufferValue );
						inletBufferValue->appendChild( bufferVal );
						basicInlet->appendChild( inletDefaultValue );
						inletDefaultValue->appendChild( defaultVal );
					}
				}
				else
				{
					AutoPtr< Text > multiValue = m_Document->createTextNode( "0" );
					isMulti->appendChild( multiValue );

					BasicInletConfig c = it->basicInlets[ 0 ];
					AutoPtr< Element > inletPolicy = m_Document->createElement( INLET_UPDATE_POLICY );
					AutoPtr< Element > inletBuffer = m_Document->createElement( INLET_BUFFER_SIZE );
					AutoPtr< Element > inletBufferValue = m_Document->createElement( INLET_BUFFER_VALUE );
					AutoPtr< Element > inletDefaultValue = m_Document->createElement( INLET_DEFAULT_VALUE );
					AutoPtr< Text > policy = m_Document->createTextNode( c.updatePolicy );
					AutoPtr< Text > buffer = m_Document->createTextNode( c.bufferSize );
					AutoPtr< Text > bufferVal = m_Document->createTextNode( c.bufferedValue );
					AutoPtr< Text > defaultVal = m_Document->createTextNode( c.initialValue );
					inlet->appendChild( inletPolicy );
					inletPolicy->appendChild( policy );
					inlet->appendChild( inletBuffer );
					inletBuffer->appendChild( buffer );
					inlet->appendChild( inletBufferValue );
					inletBufferValue->appendChild( bufferVal );
					inlet->appendChild( inletDefaultValue );
					inletDefaultValue->appendChild( defaultVal );
				}
			}
		}

		void XMLConfig::addLink( ParamConfig const& inletConfig, ParamConfig const& outletConfig )
		{
			AutoPtr< Element > link = m_Document->createElement( LINK );
			AutoPtr< Element > inlet = m_Document->createElement( INLET );
			AutoPtr< Element > inletName = m_Document->createElement( INLET_ID );
			AutoPtr< Element > inletBlock = m_Document->createElement( BLOCK_INSTANCE_ID );
			AutoPtr< Element > outlet = m_Document->createElement( OUTLET );
			AutoPtr< Element > outletName = m_Document->createElement( OUTLET_ID );
			AutoPtr< Element > outletBlock = m_Document->createElement( BLOCK_INSTANCE_ID );
			m_Links->appendChild( link );
			link->appendChild( inlet );
			link->appendChild( outlet );
			inlet->appendChild( inletName );
			inlet->appendChild( inletBlock );
			outlet->appendChild( outletName );
			outlet->appendChild( outletBlock);
			inletName->appendChild( m_Document->createTextNode( inletConfig.paramId ) );
			inletBlock->appendChild( m_Document->createTextNode( inletConfig.blockInstanceId ) );
			outletName->appendChild( m_Document->createTextNode( outletConfig.paramId ) );
			outletBlock->appendChild( m_Document->createTextNode( outletConfig.blockInstanceId ) );
		}
	}
}