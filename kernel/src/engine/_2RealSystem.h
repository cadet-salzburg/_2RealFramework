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

#include "engine/_2RealLink.h"
#include "app/_2RealCallbacks.h"
#include "app/_2RealCallbacksInternal.h"
#include "helpers/_2RealPoco.h"
#include "app/_2RealBlockHandle.h"

#include <set>

namespace _2Real
{

	class InletIO;
	class OutletIO;
	class EngineImpl;
	class AbstractLink;
	class AbstractUberBlock;
	class FunctionBlock;
	class SystemBlockManager;

	class System
	{

	public:

		System( EngineImpl &engine );
		~System();

		void		clearAll();
		void		clearBlockInstances();

		void		handleException( FunctionBlock &block, Exception const& exception );

		void		addUberBlock( AbstractUberBlock &block, const bool isContext );

		void		registerToException( app::ErrorCallback &callback );
		void		unregisterFromException( app::ErrorCallback &callback );

		void		createLink( InletIO &inlet, OutletIO &outlet );
		void		destroyLink( InletIO &inlet, OutletIO &outlet );

	private:

		EngineImpl								&m_Engine;
		SystemBlockManager						*m_SubBlockManager;
		IOLink::LinkSet							m_Links;
		CallbackEvent< std::pair< Exception, app::BlockHandle > const& >		m_ExceptionEvent;

	};

}