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

#include "_2RealLink.h"
#include "app/_2RealCallbacks.h"
#include "app/_2RealCallbacksInternal.h"

#include "_2RealPoco.h"

#include <set>

namespace _2Real
{

	class EngineImpl;
	class AbstractLink;
	class AbstractUberBlock;
	class FunctionBlock;
	class SystemBlockManager;

	class System
	{

	public:

		System( EngineImpl &engine );

		void		clear();

		void		handleException( FunctionBlock &block, Exception const& exception );

		void		addUberBlock( AbstractUberBlock &block );

		void		registerToException( app::ExceptionCallback callback, void *userData );
		void		unregisterFromException( app::ExceptionCallback callback, void *userData );
		void		registerToException( app::AbstractExceptionCallbackHandler &handler );
		void		unregisterFromException( app::AbstractExceptionCallbackHandler &handler );

		void		createLink( Inlet &inlet, Outlet &outlet );
		void		destroyLink( Inlet &inlet, Outlet &outlet );

	private:

		EngineImpl								&m_Engine;
		SystemBlockManager						*m_SubBlockManager;

		LinkSet									m_Links;

		// TODO: write own delegate class to handle this shit
		mutable Poco::FastMutex					m_ExceptionAccess;
		app::ExceptionFunctionCallbacks			m_ExceptionCallbacks;
		app::ExceptionCallbackHandlers			m_ExceptionCallbackHandlers;

	};

}