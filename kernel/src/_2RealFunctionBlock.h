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
#include "_2RealDisabledBlocks.h"
#include "_2RealFunctionBlockIOManager.h"
#include "_2RealFunctionBlockStateManager.h"

namespace _2Real
{

	class Block;
	class SystemBlock;

	class FunctionBlock : public UberBlock< FunctionBlockIOManager, DisabledBlockManager, DisabledBlockManager, FunctionBlockStateManager >
	{

	public:

		FunctionBlock( BlockData const& data, Block& block, SystemBlock &owner, BlockIdentifier const& id );

		InletHandle		createInletHandle(std::string const& inletName);
		OutletHandle	createOutletHandle(std::string const& outletName);

	private:

		FunctionBlockIOManager 		*m_IOManager;
		FunctionBlockStateManager	*m_StateManager;

	};

}