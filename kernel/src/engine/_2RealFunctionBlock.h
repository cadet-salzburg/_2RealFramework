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
#include "helpers/_2RealHandleAble.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"

namespace _2Real
{
	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
		class BlockInfo;
	}

	namespace bundle
	{
		class Block;
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
	}

	class BlockData;
	class System;
	class FunctionBlockUpdatePolicy;
	class FunctionBlockIOManager;
	class FunctionBlockStateManager;

	class FunctionBlock : public AbstractUberBlock, public HandleAble< app::BlockHandle >, public HandleAble< app::ContextBlockHandle >
	{

	public:

		FunctionBlock( BlockData const& meta, bundle::Block& block, System &system, Identifier const& id );
		~FunctionBlock();

		app::BlockInfo			getBlockData();
		BlockData const&		getMetadata() const;

		bundle::InletHandle &		getBundleInletHandle( std::string const& inletName );
		bundle::OutletHandle &		getBundleOutletHandle( std::string const& outletName );
		bundle::ParameterHandle &	getBundleParameterHandle( std::string const& paramName );

		app::InletHandle &		getAppInletHandle( std::string const& inletName );
		app::OutletHandle &		getAppOutletHandle( std::string const& outletName );
		app::ParameterHandle &	getAppParameterHandle( std::string const& paramName );

		void					createLink( Inlet &inlet, Outlet &outlet );
		void					destroyLink( Inlet &inlet, Outlet &outlet );

		void					registerToNewData( Outlet const& outlet, app::OutletCallback &callback );
		void					unregisterFromNewData( Outlet const& outlet, app::OutletCallback &callback );
		void					registerToNewData( app::BlockCallback &callback );
		void					unregisterFromNewData( app::BlockCallback &callback );

		void					setUp();
		void					start();
		void					stop( const bool blocking, const long timeout );
		void					prepareForShutDown();
		bool					shutDown( const long timeout );

		void					updateWhenInletDataNew( Inlet &inlet, const bool isSingleWeight );
		void					updateWhenInletDataValid( Inlet &inlet );
		void					updateWhenAllInletDataNew();
		void					updateWhenAllInletDataValid();
		void					updateWithFixedRate( const double updatesPerSecond );

		void					handleException( Exception &e );

	private:

		bundle::Block			&m_Block;
		System					&m_System;
		BlockData				const& m_Metadata;

		FunctionBlockStateManager	*m_StateManager;
		FunctionBlockIOManager		*m_IOManager;
		FunctionBlockUpdatePolicy	*m_UpdatePolicy;

	};
}