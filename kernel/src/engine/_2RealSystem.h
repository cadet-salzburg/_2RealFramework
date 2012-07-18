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

#include <set>

namespace _2Real
{

	class Logger;
	class AbstractUberBlock;
	template< typename T >
	class FunctionBlock;

	namespace app
	{
		class BlockHandle;
		class ContextBlockHandle;
	}

	class System
	{

	public:

		typedef std::set< AbstractUberBlock * >						Blocks;
		typedef std::set< AbstractUberBlock * >::iterator			BlockIterator;
		typedef std::set< AbstractUberBlock * >::const_iterator		BlockConstIterator;

		System( Logger &logger );
		~System();

		void			clearAll();
		void			clearBlockInstances();

		void			addBlock( FunctionBlock< app::BlockHandle > &block );
		void			addBlock( FunctionBlock< app::ContextBlockHandle > &block );
		void			removeBlock( FunctionBlock< app::BlockHandle > &block, const long timeout );
		void			removeBlock( FunctionBlock< app::ContextBlockHandle > &block, const long timeout );
		Blocks const&	getBlockInstances() const;
		Blocks const&	getBundleContexts() const;

	private:

		Logger			&m_Logger;
		Blocks			m_BlockInstances;
		Blocks			m_ContextBlocks;

	};

}