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

#include "app/_2RealBlockHandle.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	template< typename TObj >
	std::shared_ptr< TObj > checkValidity( std::weak_ptr< TObj > handle, std::string const& what )
	{
		std::shared_ptr< TObj > locked = handle.lock();
		if ( locked.get() == nullptr )
		{
			std::stringstream msg;
			msg << "nullptr access: " << what << " handle does not point to an object" << std::endl;
		}

		return locked;
	}

	namespace app
	{
		BlockHandle::BlockHandle() :
			mImpl()
		{
		}

		BlockHandle::BlockHandle( std::shared_ptr< FunctionBlock > block ) :
			mImpl( block )
		{
		}

		bool BlockHandle::isValid() const
		{
			std::shared_ptr< FunctionBlock > block = mImpl.lock();
			return ( block.get() != nullptr );
		}

		BlockInfo const& BlockHandle::getBlockInfo() const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			return block->getBlockInfo();
		}

		void BlockHandle::setUpdateRate( const double updatesPerSecond )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			block->updateWithFixedRate( updatesPerSecond );
		}

		void BlockHandle::setup()
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			block->setUp();
		}

		void BlockHandle::start()
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			block->start();
		}

		void BlockHandle::stop( const long timeout )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			block->stop( true, timeout );
		}

		void BlockHandle::destroy( const long timeout )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			block->suicide( timeout );
		}

		InletHandle BlockHandle::getInletHandle( std::string const& name )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			return InletHandle( block->getInlet( trim( name ) ) );
		}

		ParameterHandle BlockHandle::getParameterHandle( std::string const& name )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			return ParameterHandle( block->getParameter( trim( name ) ) );
		}

		OutletHandle BlockHandle::getOutletHandle( std::string const& name )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			return OutletHandle( block->getOutlet( trim( name ) ) );
		}

		void BlockHandle::getAllInletHandles( InletHandles &handles )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			FunctionBlock::Inlets &inlets = block->getAllInlets();
			handles.clear();
			for ( FunctionBlock::Inlets::iterator it = inlets.begin(); it != inlets.end(); ++it )
				handles.push_back( InletHandle( *it ) );
		}

		void BlockHandle::getAllParameterHandles( ParameterHandles &handles )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			FunctionBlock::Parameters &parameters = block->getAllParameters();
			handles.clear();
			for ( FunctionBlock::Parameters::iterator it = parameters.begin(); it != parameters.end(); ++it )
				handles.push_back( ParameterHandle( *it ) );
		}

		void BlockHandle::getAllOutletHandles( OutletHandles &handles )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			FunctionBlock::Outlets &outlets = block->getAllOutlets();
			handles.clear();
			for ( FunctionBlock::Outlets::iterator it = outlets.begin(); it != outlets.end(); ++it )
				handles.push_back( OutletHandle( *it ) );
		}

		void BlockHandle::registerToNewData( BlockDataCallback callback, void *userData ) const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			BlockCallback *cb = new FunctionCallback< std::vector< std::shared_ptr< const CustomType > > >( callback, userData );
			block->registerToNewData( *cb );
		}

		void BlockHandle::unregisterFromNewData( BlockDataCallback callback, void *userData ) const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			BlockCallback *cb = new FunctionCallback< std::vector< std::shared_ptr< const CustomType > > >( callback, userData );
			block->unregisterFromNewData( *cb);
		}

		void BlockHandle::registerToNewDataInternal( BlockCallback &cb ) const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			block->registerToNewData( cb );
		}

		void BlockHandle::unregisterFromNewDataInternal( BlockCallback &cb ) const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "regular block" );
			block->unregisterFromNewData( cb );
		}

		//void BlockHandle::singleStep()
		//{
		//	checkValidity( mImpl );
		//	mImpl->singleStep();
		//}

		//bool BlockHandle::isRunning() const
		//{
		//	checkValidity( mImpl );
		//	return mImpl->isRunning();
		//}

		//std::string const& BlockHandle::getIdAsString() const
		//{
		//	checkValidity( mImpl );
		//	return mImpl->getName();
		//}
	}
}
