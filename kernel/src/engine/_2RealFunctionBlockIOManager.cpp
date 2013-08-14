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
#include "engine/_2RealFunctionBlockUpdateManager.h"
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
#include "engine/_2RealIOMetadata.h"
#include "engine/_2RealFunctionBlock.h"

namespace _2Real
{
	template< typename TObj >
	std::shared_ptr< TObj > findByName( std::vector< std::shared_ptr< TObj > > const& collection, std::string const& name, std::string const& what )
	{
		for ( std::vector< std::shared_ptr< TObj > >::const_iterator it = collection.begin(); it != collection.end(); ++it )
		{
			if ( toLower( ( *it )->getMetadata()->identifier->getObjectName() ) == toLower( name ) )
				return *it;
		}

		std::ostringstream msg;
		msg << what << " " << name<< " was not found";// in" << mOwner->getFullName();
		throw NotFoundException( msg.str() );
	}

	FunctionBlockIOManager::FunctionBlockIOManager( EngineImpl *engine, FunctionBlock *owner ) :
		AbstractIOManager(),
		mEngineImpl( engine ),
		mOwningBlock( owner )
	{
	}

	FunctionBlockIOManager::~FunctionBlockIOManager()
	{
	}

	std::string const& FunctionBlockIOManager::getFullHumanReadableName() const
	{
		return mOwningBlock->getFullHumanReadableName();
	}

	std::string const& FunctionBlockIOManager::getHumanReadableName() const
	{
		return mOwningBlock->getHumanReadableName();
	}

	//std::string const& FunctionBlockIOManager::getCode() const
	//{
	//	return mOwningBlock->getCode();
	//}

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

	std::shared_ptr< AbstractInletIO > FunctionBlockIOManager::getParameter( std::string const& name )
	{
		return findByName< AbstractInletIO >( mParameters, name, "parameter" );
	}

	std::shared_ptr< const AbstractInletIO > FunctionBlockIOManager::getInlet( std::string const& name ) const
	{
		return findByName< AbstractInletIO >( mInlets, name, "inlet" );
	}

	std::shared_ptr< const OutletIO > FunctionBlockIOManager::getOutlet( std::string const& name ) const
	{
		return findByName< OutletIO >( mOutlets, name, "outlet" );
	}

	std::shared_ptr< const AbstractInletIO > FunctionBlockIOManager::getParameter( std::string const& name ) const
	{
		return findByName< AbstractInletIO >( mParameters, name, "parameter" );
	}

	void FunctionBlockIOManager::addInlet( std::shared_ptr< const IOMetadata > meta )
	{
		if ( meta->canExpand )
		{
			assert( NULL );
			//Identifier id = Identifier::makeInletId( mOwningBlock->getIdentifier(), meta->name, mInlets.size() );
			//MultiInletIO *io = new MultiInletIO( mEngineImpl, id, mOwningBlock, mUpdatePolicy, meta );
			//std::shared_ptr< AbstractInletIO > shared( io );
			//mInlets.push_back( shared );
		}
		else
		{
			std::shared_ptr< InstanceId > id = IdGenerator::makeInletId( meta->identifier, mOwningBlock->getIdentifier() );
			BasicInletIO *io = new BasicInletIO( mEngineImpl, id, mOwningBlock, mUpdateManager.lock(), meta );
			std::shared_ptr< AbstractInletIO > shared( io );
			std::shared_ptr< BasicInletIO > basic = std::static_pointer_cast< BasicInletIO >( shared );
			io->setSelfRef( basic );
			mInlets.push_back( shared );
			mUpdateManager.lock()->inletAdded( basic, meta->updatePolicy );
		}
	}

	void FunctionBlockIOManager::addParameter( std::shared_ptr< const IOMetadata > meta )
	{
		std::shared_ptr< InstanceId > id = IdGenerator::makeParameterId( meta->identifier, mOwningBlock->getIdentifier() );
		BasicInletIO *io = new BasicInletIO( mEngineImpl, id, mOwningBlock, mUpdateManager.lock(), meta );
		std::shared_ptr< AbstractInletIO > shared( io );
		std::shared_ptr< BasicInletIO > basic = std::static_pointer_cast< BasicInletIO >( shared );
		io->setSelfRef( basic );
		mParameters.push_back( shared );
		mUpdateManager.lock()->inletAdded( basic, meta->updatePolicy );
	}

	void FunctionBlockIOManager::addOutlet( std::shared_ptr< const IOMetadata > meta )
	{
		std::shared_ptr< InstanceId > id = IdGenerator::makeOutletId( meta->identifier, mOwningBlock->getIdentifier() );
		std::shared_ptr< OutletIO > io( new OutletIO( mEngineImpl, id, mOwningBlock, meta ) );
		io->setSelfRef( io );
		io->synchronizeData();											// now outlet holds a clone of the init value, while buffer is empty
		mOutlets.push_back( io );
	}

	void FunctionBlockIOManager::updateInletData()
	{
		for ( InletIterator it = mInlets.begin(); it != mInlets.end(); ++it )
			( *it )->synchronize();
	}

	void FunctionBlockIOManager::updateParameterData()
	{
		for ( ParameterIterator it = mParameters.begin(); it != mParameters.end(); ++it )
			( *it )->synchronize();
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

}