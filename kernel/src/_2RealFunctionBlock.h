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

#include "_2RealUberBlock.h"
#include "_2RealFunctionBlockIOManager.h"
#include "_2RealFunctionBlockStateManager.h"

namespace _2Real
{
	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
	}

	namespace bundle
	{
		class Block;
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
	}

	class BlockData;
	class SystemBlock;

	class FunctionBlock : public UberBlock< FunctionBlockIOManager, FunctionBlockStateManager >
	{

	public:

		using UberBlock::start;
		using UberBlock::stop;
		using UberBlock::setUp;

		FunctionBlock( BlockData const& data, bundle::Block& block, SystemBlock &system, BlockIdentifier const& id );

		BlockData const&		getMetadata();

		bundle::InletHandle		createBundleInletHandle( std::string const& inletName );
		bundle::OutletHandle	createBundleOutletHandle( std::string const& outletName );
		bundle::ParameterHandle	createBundleParameterHandle( std::string const& paramName );

		app::InletHandle		createAppInletHandle( std::string const& inletName );
		app::OutletHandle		createAppOutletHandle( std::string const& outletName );
		app::ParameterHandle	createAppParameterHandle( std::string const& paramName );

		void					createLink( Inlet &inlet, Outlet &outlet );
		void					destroyLink( Inlet &inlet, Outlet &outlet );

	private:

		bundle::Block	&m_Block;
		SystemBlock		&m_System;
		BlockData		const& m_BlockData;

	};
}