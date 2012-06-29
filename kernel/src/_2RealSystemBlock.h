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
#include "_2RealDisabledIOManager.h"
#include "_2RealSystemBlockStateManager.h"
#include "_2RealLink.h"

#include <set>

namespace _2Real
{

	class EngineImpl;
	class AbstractLink;
	class SystemBlockManager;

	class SystemBlock : public UberBlock< DisabledIOManager, SystemBlockStateManager >
	{

	public:

		SystemBlock( EngineImpl &engine, BlockIdentifier const& id );

		// wipes the system completely
		// all blocks, links etc. will be gone
		void		clear();

		// called by a block ( other that the system ) when an exception occurs
		void		handleException( AbstractUberBlock &block, Exception &exception );

		// adds block to the system, yay
		void		addUberBlock( AbstractUberBlock &block );

		// will be called by the engine
		//void		registerToException( ExceptionCallback callback, void *userData );
		//void		unregisterFromException( ExceptionCallback callback, void *userData );
		//void		registerToException( AbstractExceptionCallbackHandler &handler );
		//void		unregisterFromException( AbstractExceptionCallbackHandler &handler );

		// yay
		void		createLink( Inlet &inlet, Outlet &outlet );
		void		destroyLink( Inlet &inlet, Outlet &outlet );

	private:

		EngineImpl								&m_Engine;
		SystemBlockManager						*m_SubBlockManager;

		LinkSet									m_Links;

		// TODO: write own delegate class to handle this shit
		//mutable Poco::FastMutex					m_ExceptionAccess;
		//ExceptionFunctionCallbacks				m_ExceptionCallbacks;
		//ExceptionCallbackHandlers				m_ExceptionCallbackHandlers;

	};

}