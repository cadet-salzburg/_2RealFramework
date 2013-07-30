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

#include <map>
#include <memory>
#include <string>

namespace _2Real
{

	class EngineImpl;
	class AbstractUberBlock;
	class FunctionBlock;
	class Bundle;

	class System
	{

	public:

		typedef std::multimap< std::string, std::shared_ptr< AbstractUberBlock > >					Blocks;
		typedef std::multimap< std::string, std::shared_ptr< AbstractUberBlock > >::iterator		BlockIterator;
		typedef std::multimap< std::string, std::shared_ptr< AbstractUberBlock > >::const_iterator	BlockConstIterator;
		typedef std::map< std::string, std::shared_ptr< AbstractUberBlock > >						ContextBlocks;
		typedef std::map< std::string, std::shared_ptr< AbstractUberBlock > >::iterator				ContextBlockIterator;
		typedef std::map< std::string, std::shared_ptr< AbstractUberBlock > >::const_iterator		ContextBlockConstIterator;

		System( EngineImpl * );
		~System();

		void			clear();

		void			addRegularBlockInstance( Bundle *, std::shared_ptr< FunctionBlock > );
		void			addContextBlockInstance( Bundle *, std::shared_ptr< FunctionBlock > );

		void								destroyBlocks( Bundle *bundle );
		unsigned int						getBlockInstanceCount( Bundle const* bundle, std::string const& );
		std::shared_ptr< FunctionBlock >	getContextBlock( Bundle const* bundle );


	private:

		EngineImpl		*const mEngineImpl;
		Blocks			mBlockInstances;
		ContextBlocks	mContextBlocks;

	};

}