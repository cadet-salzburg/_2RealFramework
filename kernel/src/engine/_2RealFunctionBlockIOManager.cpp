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

#include "engine/_2RealFunctionBlockIOManager.h"
#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealInletBuffer.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "helpers/_2RealStringHelpers.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealIOMetadata.h"

namespace _2Real
{
	template< typename TObj >
	std::shared_ptr< TObj > findByName( std::vector< std::shared_ptr< TObj > > const& collection, std::string const& name, std::string const& what )
	{
		for ( std::vector< std::shared_ptr< TObj > >::const_iterator it = collection.begin(); it != collection.end(); ++it )
		{
			if ( toLower( ( *it )->getInfo()->name ) == toLower( name ) )
				return *it;
		}

		std::ostringstream msg;
		msg << what << " " << name<< " was not found";// in" << mOwner->getFullName();
		throw NotFoundException( msg.str() );
	}

	FunctionBlockIOManager::FunctionBlockIOManager( EngineImpl *engine, AbstractUberBlock *owner ) :
		AbstractIOManager( engine, owner )
	{
	}

	FunctionBlockIOManager::~FunctionBlockIOManager()
	{
	}

	void FunctionBlockIOManager::registerToNewData( AbstractCallback< std::vector< std::shared_ptr< const CustomType > > > &cb )
	{
		m_AppEvent.addListener( cb );
	}

	void FunctionBlockIOManager::unregisterFromNewData( AbstractCallback< std::vector< std::shared_ptr< const CustomType > > > &cb )
	{
		m_AppEvent.removeListener( cb );
	}

	AbstractIOManager::InletVector & FunctionBlockIOManager::getAllInlets()
	{
		return mInlets;
	}

	AbstractIOManager::OutletVector & FunctionBlockIOManager::getAllOutlets()
	{
		return mOutlets;
	}

	AbstractIOManager::ParameterVector & FunctionBlockIOManager::getAllParameters()
	{
		return mParameters;
	}

	AbstractIOManager::InletVector const& FunctionBlockIOManager::getAllInlets() const
	{
		return mInlets;
	}

	AbstractIOManager::OutletVector const& FunctionBlockIOManager::getAllOutlets() const
	{
		return mOutlets;
	}

	AbstractIOManager::ParameterVector const& FunctionBlockIOManager::getAllParameters() const
	{
		return mParameters;
	}

	std::shared_ptr< AbstractInletIO > FunctionBlockIOManager::getInlet( std::string const& name )
	{
		return findByName< AbstractInletIO >( mInlets, name, "inlet" );
	}

	std::shared_ptr< OutletIO > FunctionBlockIOManager::getOutlet( std::string const& name )
	{
		return findByName< OutletIO >( mOutlets, name, "outlet" );
	}

	std::shared_ptr< ParameterIO > FunctionBlockIOManager::getParameter( std::string const& name )
	{
		return findByName< ParameterIO >( mParameters, name, "parameter" );
	}

	std::shared_ptr< const AbstractInletIO > FunctionBlockIOManager::getInlet( std::string const& name ) const
	{
		return findByName< AbstractInletIO >( mInlets, name, "inlet" );
	}

	std::shared_ptr< const OutletIO > FunctionBlockIOManager::getOutlet( std::string const& name ) const
	{
		return findByName< OutletIO >( mOutlets, name, "outlet" );
	}

	std::shared_ptr< const ParameterIO > FunctionBlockIOManager::getParameter( std::string const& name ) const
	{
		return findByName< ParameterIO >( mParameters, name, "parameter" );
	}

	void FunctionBlockIOManager::addInlet( std::shared_ptr< const IOMetadata > meta )
	{
		if ( meta->canExpand )
		{
			MultiInletIO *io = new MultiInletIO( mEngineImpl, mOwner, m_UpdatePolicy, meta );
			std::shared_ptr< AbstractInletIO > shared( io );
			mInlets.push_back( shared );
		}
		else
		{
			BasicInletIO *io = new BasicInletIO( mEngineImpl, mOwner, m_UpdatePolicy, meta );
			std::shared_ptr< AbstractInletIO > shared( io );
			io->setSelfRef( std::static_pointer_cast< BasicInletIO >( shared ) );
			mInlets.push_back( std::shared_ptr< AbstractInletIO >( shared ) );
		}
	}

	void FunctionBlockIOManager::addParameter( std::shared_ptr< const IOMetadata > meta )
	{
		std::shared_ptr< ParameterIO > io( new ParameterIO( mEngineImpl, mOwner, meta ) );
		io->setData( meta->initializer );								// buffer now holds init value
		io->synchronizeData();											// parameter data now holds value
		io->synchronizeData();											// parameter now will retun hasChanged() -> false
		mParameters.push_back( io );
	}

	void FunctionBlockIOManager::addOutlet( std::shared_ptr< const IOMetadata > meta )
	{
		std::shared_ptr< OutletIO > io( new OutletIO( mEngineImpl, mOwner, meta ) );
		io->setSelfRef( io );
		io->synchronizeData();											// now outlet holds a clone of the init value, while buffer is empty
		mOutlets.push_back( io );
	}

	void FunctionBlockIOManager::updateInletData()
	{
		for ( InletIterator it = mInlets.begin(); it != mInlets.end(); ++it )
			( *it )->synchronizeData();
	}

	void FunctionBlockIOManager::updateParameterData()
	{
		for ( ParameterIterator it = mParameters.begin(); it != mParameters.end(); ++it )
			( *it )->synchronizeData();
	}

	void FunctionBlockIOManager::updateOutletData()
	{
		std::vector< std::shared_ptr< const CustomType > > blockData;
		for ( OutletIterator it = mOutlets.begin(); it != mOutlets.end(); ++it )
		{
			std::shared_ptr< const CustomType > data = ( *it )->synchronizeData();
			if ( data.get() ) blockData.push_back( data );
		}

		if ( !blockData.empty() )
			m_AppEvent.notify( blockData );
	}

	void FunctionBlockIOManager::updateInletBuffers( const bool enableTriggering )
	{
		for ( InletIterator it = mInlets.begin(); it != mInlets.end(); ++it )
		{
			for ( unsigned int i = 0; i<( *it )->getSubInletCount(); ++i )
			{
				std::shared_ptr< BasicInletIO > io = ( **it )[ i ];
				io->processQueue();
			}
		}
	}

}