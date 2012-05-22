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

#include "_2RealAbstractUberBlockManager.h"

namespace _2Real
{

	class DisabledBlocks : public AbstractBlockManager
	{

	public:

		DisabledBlocks(AbstractBlock &owner);
		~DisabledBlocks();

		void								clear();
		AbstractBlock &						getBlock(Identifier const& blockId);
		AbstractBlock const&				getBlock(Identifier const& blockId) const;
		void								addBlock(AbstractBlock &block);
		void								removeBlock(AbstractBlock &block);
		std::list< Identifier >				getCurrentBlockIds() const;

	};

}