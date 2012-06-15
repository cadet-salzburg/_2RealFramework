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
#include "_2RealServiceBlockIO.h"
#include "_2RealServiceBlockStates.h"
#include "_2RealServiceBlockData.h"

namespace _2Real
{

	class Block;
	class SystemImpl;
	class UpdatePolicyImpl;

	class FunctionBlock : public UberBlock< ServiceIO, DisabledBlocks, DisabledBlocks, FunctionBlockStateManager >
	{

	public:

		FunctionBlock( BlockData const& data, Block& block, SystemImpl &owner, std::string const& name );

		InletHandle		createInletHandle(std::string const& inletName);
		OutletHandle	createOutletHandle(std::string const& outletName);

		Block * getBlock() { return &m_Block; }

	private:

		Block			&m_Block;

	};

}