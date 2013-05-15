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

#include "_2RealConfigLoader.h"
#include "_2RealSystemState.h"
#include "engine\_2RealEngineImpl.h"

#include "engine\_2RealBundle.h"			// for create block
#include "engine\_2RealFunctionBlock.h"		// for setup, start, getInlet etc

namespace _2Real
{
	app::SystemState * ConfigurationLoader::load( EngineImpl &engine, Configuration &config ) const
	{
		std::cout << "BEGIN LOADING A CONFIGURATION!" << std::endl;

		/**
		*	todo: catch all exceptions, transform into invalid config exception
		**/

		app::SystemState *state = new app::SystemState();

		Configuration::BundleConfigurations &bundles = config.bundleConfigs();
		Configuration::BlockConfigurations &configs = config.blockConfigs();
		Configuration::LinkConfigurations &links = config.linkConfigs();

		std::cout << "------BUNDLES------" << std::endl;

		/* resolve bundles */
		for ( Configuration::BundleConfigurations::iterator bundleIter = bundles.begin(); bundleIter != bundles.end(); ++bundleIter )
		{
			std::cout << "looking for bundle: " << bundleIter->first << std::endl;

			// T: might throw if bundle not found
			bundleIter->second.bundle = &( engine.findBundleByName( bundleIter->first ) );
		}

		std::cout << "------BLOCKS-----" << std::endl;

		for ( Configuration::BlockConfigurations::iterator blockIter = configs.begin(); blockIter != configs.end(); ++blockIter )
		{
			Configuration::BlockConfig &blockConfiguration = blockIter->second;
			Configuration::BundleConfigurations::iterator bundleIter = bundles.find( blockConfiguration.bundleId );

			// T: might throw if bundle does not contain block
			if ( bundleIter != bundles.end() && bundleIter->second.bundle != nullptr )
				blockConfiguration.block = &( bundleIter->second.bundle->createBlockInstance( blockConfiguration.blockId ) );
			else
			{
				std::stringstream msg;
				msg << "the bundle " << blockConfiguration.bundleId << " is not loaded!" << std::endl;
				throw NotFoundException( msg.str() );
			}

			/* add to system state */
			app::BlockHandle h = blockConfiguration.block->getHandle();
			state->mVertices[ h ] = new app::SystemState::Vertex( h );
			blockConfiguration.block->updateWithFixedRate( Configuration::toDouble( blockConfiguration.fps ) );

			std::cout << "handling the inlets" << std::endl;

			/* next: 'set up' all inlets */
			for ( std::map< std::string, Configuration::InletConfig >::iterator inletIter = blockConfiguration.inlets.begin(); inletIter != blockConfiguration.inlets.end(); ++inletIter )
			{
				Configuration::InletConfig &inletConfiguration = inletIter->second;
				inletConfiguration.inlet = &( blockConfiguration.block->getInlet( inletConfiguration.inletId ) );

				std::cout << inletConfiguration.inletId << std::endl;

				if ( Configuration::toBool( inletConfiguration.isMultiinlet ) )
				{
					for ( std::vector< Configuration::BasicInletConfig >::iterator basicIter = inletConfiguration.basicInlets.begin(); basicIter != inletConfiguration.basicInlets.end(); ++basicIter )
					{
						Configuration::BasicInletConfig &basicConfiguration = *basicIter;

						// T: might throw if not a multiinlet
						basicConfiguration.inlet = dynamic_cast< BasicInletIO * >( inletConfiguration.inlet->addBasicInlet() );
						basicConfiguration.inlet->setBufferSize( Configuration::toUnsignedInt( basicConfiguration.bufferSize ) );
						std::cout << "buffer size: " << Configuration::toUnsignedInt( basicConfiguration.bufferSize ) << std::endl;
						basicConfiguration.inlet->setUpdatePolicy( InletPolicy::getPolicyFromString( basicConfiguration.updatePolicy ) );
						std::cout << "policy: " << basicConfiguration.updatePolicy << std::endl;
					}
				}
				else
				{
					Configuration::BasicInletConfig &basicConfiguration = inletConfiguration.basicInlets.front();
					basicConfiguration.inlet = dynamic_cast< BasicInletIO * >( inletConfiguration.inlet );
					basicConfiguration.inlet->setBufferSize( Configuration::toUnsignedInt( basicConfiguration.bufferSize ) );
					std::cout << "buffer size: " << Configuration::toUnsignedInt( basicConfiguration.bufferSize ) << std::endl;
					basicConfiguration.inlet->setUpdatePolicy( InletPolicy::getPolicyFromString( basicConfiguration.updatePolicy ) );
					std::cout << "policy: " << basicConfiguration.updatePolicy << std::endl;
				}
			}

			/* since all setup values are set now, I can call setup */
			blockConfiguration.block->setUp();
			std::cout << "set up block: " << blockConfiguration.blockInstanceId << std::endl;

			/* now set the inlet values */
			for ( std::map< std::string,Configuration::InletConfig >::iterator inletIter = blockConfiguration.inlets.begin(); inletIter != blockConfiguration.inlets.end(); ++inletIter )
			{
				Configuration::InletConfig &inletConfiguration = inletIter->second;

				for ( std::vector<Configuration::BasicInletConfig >::iterator basicIter = inletConfiguration.basicInlets.begin(); basicIter != inletConfiguration.basicInlets.end(); ++basicIter )
				{
					Configuration::BasicInletConfig &basicConfiguration = *basicIter;

					if ( basicConfiguration.inlet->info().type.m_TypeName == "string" )
					{
						std::cout << "rec string: " << basicConfiguration.inletId << std::endl;
						std::cout << basicConfiguration.bufferedValue << std::endl;
						basicConfiguration.inlet->receiveData( Any( basicConfiguration.bufferedValue ) );
					}
					else if ( basicConfiguration.inlet->info().type.m_LongTypename.find( "vector" ) != std::string::npos )
						basicConfiguration.inlet->receiveData( basicConfiguration.bufferedValue );
					else if ( basicConfiguration.bufferedValue.find( "UNSTREAMABLE" ) != std::string::npos )
						std::cout << "found unreadable buffered value for " << basicConfiguration.inlet->info().baseName << std::endl;
					else
					{
						std::cout << "rec any: " << basicConfiguration.inletId << std::endl;
						std::cout << basicConfiguration.bufferedValue << std::endl;
						basicConfiguration.inlet->receiveData( basicConfiguration.bufferedValue );
					}
				}
			}
		}

		std::cout << "------LINKS------" << std::endl;

		for ( Configuration::BlockConfigurations::iterator blockIter = configs.begin(); blockIter != configs.end(); ++blockIter )
		{
			Configuration::BlockConfig &blockConfiguration = blockIter->second;

			for ( std::map< std::string,Configuration::InletConfig >::iterator inletIter = blockConfiguration.inlets.begin(); inletIter != blockConfiguration.inlets.end(); ++inletIter )
			{
				Configuration::InletConfig &inletConfiguration = inletIter->second;

				for ( std::vector<Configuration::BasicInletConfig >::iterator basicIter = inletConfiguration.basicInlets.begin(); basicIter != inletConfiguration.basicInlets.end(); ++basicIter )
				{
					Configuration::BasicInletConfig &basicConfiguration = *basicIter;

					/* look into link map, to see whether blockInstanceID & inletID are stored there */
					Configuration::ParamConfig linkIn;
					linkIn.blockInstanceId = blockConfiguration.blockInstanceId;
					linkIn.paramId = basicConfiguration.inletId;

					std::pair< std::multimap< Configuration::ParamConfig,Configuration::ParamConfig >::iterator, std::multimap<Configuration::ParamConfig,Configuration::ParamConfig >::iterator > range = links.equal_range( linkIn );
					for ( std::multimap< Configuration::ParamConfig,Configuration::ParamConfig >::iterator linkIter = range.first; linkIter != range.second; ++linkIter )
					{
						Configuration::ParamConfig &linkOut = linkIter->second;
						Configuration::BlockConfig &out = configs[ linkOut.blockInstanceId ];

						std::cout << "inlet: " << linkIn.blockInstanceId << " " << linkIn.paramId << std::endl;
						std::cout << "is linked with: " << linkOut.blockInstanceId << " " << linkOut.paramId << std::endl;
						OutletIO *outlet = &( out.block->getOutlet( linkOut.paramId ) );

						bool linked = false;
						IOLink link = engine.createLink( *( basicConfiguration.inlet ), *outlet );
						if ( !link.isValid() )
						{
							std::pair< IOLink, IOLink > links = engine.createLinkWithConversion( *( basicConfiguration.inlet ), *outlet );
							linked = links.first.isValid();
						}
						else
						{
							linked = true;
						}

						// both blockHandles are now for sure in the graph's vertices
						app::SystemState::Vertex *i = state->mVertices[ blockConfiguration.block->getHandle() ];
						app::SystemState::Vertex *o = state->mVertices[ out.block->getHandle() ];

						state->mEdges.insert( new app::SystemState::Edge( app::Engine::Link( basicConfiguration.inlet->getHandle(), outlet->getHandle() ), *i, *o ) );
					}
				}
			}
		}

		for ( Configuration::BlockConfigurations::iterator blockIter = configs.begin(); blockIter != configs.end(); ++blockIter )
		{
			Configuration::BlockConfig &blockConfiguration = blockIter->second;
			blockConfiguration.block->start();
			std::cout << "started block: " << blockConfiguration.block->getBlockInfo().name << std::endl;
		}

		std::cout << "FINISHED LOADING A CONFIGURATION!" << std::endl;

		return state;
	}
}