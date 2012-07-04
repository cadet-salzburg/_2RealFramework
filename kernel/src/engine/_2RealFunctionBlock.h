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
#include "engine/_2RealAbstractIOManager.h"

namespace _2Real
{
	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class BlockInfo;
	}

	namespace bundle
	{
		class Block;
		class InletHandle;
		class OutletHandle;
	}

	class BlockData;
	class System;
	class InletIO;
	class FunctionBlockUpdatePolicy;
	class FunctionBlockIOManager;
	class FunctionBlockStateManager;

	class FunctionBlock : public AbstractUberBlock, public HandleAble< app::BlockHandle >, public HandleAble< app::ContextBlockHandle >
	{

	public:

		FunctionBlock( BlockData const& meta, bundle::Block& block, Identifier const& id );
		~FunctionBlock();

		app::BlockInfo				getBlockData();
		BlockData const&			getMetadata() const;

		bundle::InletHandle &		getBundleInletHandle( std::string const& inletName ) const;
		bundle::OutletHandle &		getBundleOutletHandle( std::string const& outletName ) const;
		app::InletHandle &			getAppInletHandle( std::string const& inletName ) const;
		app::OutletHandle &			getAppOutletHandle( std::string const& outletName ) const;

		AppInletHandles const&		getAppInletHandles() const;
		AppOutletHandles const&		getAppOutletHandles() const;
		BundleInletHandles const&	getBundleInletHandles() const;
		BundleOutletHandles const&	getBundleOutletHandles() const;

		void						registerToNewData( app::BlockCallback &callback );
		void						unregisterFromNewData( app::BlockCallback &callback );

		void						setUp();
		void						start();
		void						stop( const bool blocking, const long timeout );
		void						prepareForShutDown();
		bool						shutDown( const long timeout );

		void						updateWhenInletDataNew( InletIO &inletIO, const bool isSingleWeight );
		void						updateWhenInletDataValid( InletIO &inletIO );
		void						updateWithFixedRate( const double updatesPerSecond );

		void						handleException( Exception &e );

	private:

		EngineImpl					&m_Engine;
		bundle::Block				&m_Block;
		BlockData					const& m_Metadata;

		FunctionBlockStateManager	*m_StateManager;
		FunctionBlockIOManager		*m_IOManager;
		FunctionBlockUpdatePolicy	*m_UpdatePolicy;

	};
}