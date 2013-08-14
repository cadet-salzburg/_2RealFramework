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

#include "engine/_2RealAbstractUberBlock.h"
#include "helpers/_2RealIdentifiable.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class EngineImpl;
	class Timer;

	class AbstractInletIO;
	class OutletIO;

	class Bundle;
	class BlockMetadata;
	class IOMetadata;

	class FunctionBlockUpdateManager;
	class FunctionBlockStateManager;
	class FunctionBlockIOManager;

	template< typename TArg >
	class AbstractCallback;
	class CustomType;

	namespace bundle
	{
		class Block;
	}

	class FunctionBlock : public AbstractUberBlock
	{

	public:

		typedef std::vector< std::shared_ptr< AbstractInletIO > >						Inlets;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::iterator				InletIterator;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::const_iterator		InletConstIterator;

		typedef std::vector< std::shared_ptr< OutletIO > >								Outlets;
		typedef std::vector< std::shared_ptr< OutletIO > >::iterator					OutletIterator;
		typedef std::vector< std::shared_ptr< OutletIO > >::const_iterator				OutletConstIterator;

		typedef std::vector< std::shared_ptr< AbstractInletIO > >						Parameters;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::iterator				ParameterIterator;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::const_iterator		ParameterConstIterator;

		FunctionBlock( EngineImpl *, std::shared_ptr< InstanceId > id, std::shared_ptr< const Bundle >, std::shared_ptr< bundle::Block >, std::shared_ptr< const BlockMetadata > );
		~FunctionBlock();

		std::string const&						getFullHumanReadableName() const;
		std::string const&						getHumanReadableName() const;
		//std::string const&						getCode() const;
		std::shared_ptr< const InstanceId > 						getIdentifier() const;

		void									setSelfRef( std::shared_ptr< FunctionBlock > );
		std::shared_ptr< FunctionBlock >		getSelfRef();
		std::shared_ptr< const FunctionBlock >	getSelfRef() const;

		bool									isContext() const;

		std::shared_ptr< const BlockMetadata >	getBlockMetadata() const;

		std::shared_ptr< AbstractInletIO >		getInlet( std::string const& );
		std::shared_ptr< OutletIO >				getOutlet( std::string const& );
		std::shared_ptr< AbstractInletIO >		getParameter( std::string const& );

		Inlets &								getAllInlets();
		Outlets &								getAllOutlets();
		Parameters &							getAllParameters();

		void									registerToNewData( AbstractCallback< std::vector< std::shared_ptr< const CustomType > > > &callback );
		void									unregisterFromNewData( AbstractCallback< std::vector< std::shared_ptr< const CustomType > > > &callback );

		void									setUp();
		void									start();
		void									stop( const bool blocking, const long timeout );
		void									prepareForShutDown();
		bool									shutDown( const long timeout );
		void									singleStep();
		void									suicide( const long timeout );

		void									setUpdateTimer( std::shared_ptr< Timer > );

		void									handleException( Exception const& e );

		void									addInlet( std::shared_ptr< const IOMetadata > );
		void									addOutlet( std::shared_ptr< const IOMetadata > );
		void									addParameter( std::shared_ptr< const IOMetadata > );

	private:

		EngineImpl										*const mEngineImpl;

		std::shared_ptr< InstanceId > 		mIdentifier;

		std::shared_ptr< FunctionBlockUpdateManager >	mUpdateManager;
		std::shared_ptr< FunctionBlockIOManager >		mIOManager;				// handled by bundle::block handle
		std::shared_ptr< FunctionBlockStateManager >	mStateManager;

		std::weak_ptr< const Bundle >					mBundle;				// weak ptr b/c of circualr referencing
		std::weak_ptr< FunctionBlock >					mSelfRef;				// self ref

		std::shared_ptr< bundle::Block >				mBlock;					// the only pointer to the actual block instance....
		std::shared_ptr< const BlockMetadata >			mBlockMetadata;

		std::shared_ptr< Timer >						mUpdateTimer;

	};
}
