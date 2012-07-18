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
#include "app/_2RealBundleHandle.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"

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
#define INLET_CONFIGURATION		"inlet_configuration"
#define INLET_CONFIGURATIONS	"inlet_configurations"
#define OUTLET					"outlet"
#define SETTINGS				"settings"
#define UPDATE_RATE				"fps"
#define INLET_UPDATE_POLICY		"policy"
#define INLET_BUFFER_SIZE		"buffer"
#define INLET_ID				"inlet_id"
#define INLET_VALUE				"value"
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

			map< string, app::BlockHandle > blockInstances;
			map< string, bool > blockStates;
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

					blockHandle.setUpdateRate( updateRate );

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
							Element *updatePolicy = getChildElementByName( INLET_UPDATE_POLICY, *inletConfiguration );
							inConfig.updatePolicy = trim( updatePolicy->innerText() );
							Element *bufferSize = getChildElementByName( INLET_BUFFER_SIZE, *inletConfiguration );
							inConfig.bufferSize = trim( bufferSize->innerText() );
							config.inlets.push_back( inConfig );

							Element *value = getChildElementByName( INLET_VALUE, *inletConfiguration );
							inConfig.value = trim( value->innerText() );

							app::InletHandle inletHandle = blockHandle.getInletHandle( inConfig.inletId );
							if ( !inConfig.value.empty() ) inletHandle.setValueToString( inConfig.value );

							stringstream sBuffer;
							unsigned int buffer;
							sBuffer << inConfig.bufferSize;
							sBuffer >> buffer;

							inletHandle.setBufferSize( buffer );
							if ( inConfig.updatePolicy == "valid_data" )
							{
								inletHandle.setUpdatePolicy( app::InletHandle::ALWAYS );
							}
							else if ( inConfig.updatePolicy == "or_newer_data" )
							{
								inletHandle.setUpdatePolicy( app::InletHandle::OR_NEWER_DATA );
							}
							else if ( inConfig.updatePolicy == "and_newer_data" )
							{
								inletHandle.setUpdatePolicy( app::InletHandle::AND_NEWER_DATA );
							}
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

					if ( inIt != blockInstances.end() && outIt != blockInstances.end() && inIt->second.isValid() && outIt->second.isValid() )
					{
						app::InletHandle inHandle = inIt->second.getInletHandle( inletConfig.paramId );
						app::OutletHandle outHandle = outIt->second.getOutletHandle( outletConfig.paramId );

						inHandle.linkTo( outHandle );
					}
				}
			}
			createdLinks->release();

			for ( map< string, app::BlockHandle >::iterator it = blockInstances.begin(); it != blockInstances.end(); ++it )
			{
				it->second.setup();
				if ( blockStates[ it->first ] )
				{
					it->second.start();
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
			AutoPtr< Text > path = m_Document->createTextNode( config.bundlePath );
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
				AutoPtr< Element > inletPolicy = m_Document->createElement( INLET_UPDATE_POLICY );
				AutoPtr< Element > inletBuffer = m_Document->createElement( INLET_BUFFER_SIZE );
				AutoPtr< Element > inletValue = m_Document->createElement( INLET_VALUE );
				AutoPtr< Text > name = m_Document->createTextNode( it->inletId );
				AutoPtr< Text > policy = m_Document->createTextNode( it->updatePolicy );
				AutoPtr< Text > buffer = m_Document->createTextNode( it->bufferSize );
				AutoPtr< Text > val = m_Document->createTextNode( it->value );
				inlet->appendChild( inletName );
				inletName->appendChild( name );
				inlet->appendChild( inletPolicy );
				inletPolicy->appendChild( policy );
				inlet->appendChild( inletBuffer );
				inletBuffer->appendChild( buffer );
				inlet->appendChild( inletValue );
				inletValue->appendChild( val );
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