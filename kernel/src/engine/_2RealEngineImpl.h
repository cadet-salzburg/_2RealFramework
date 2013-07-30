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

#include "app/_2RealCallbacks.h"
#include "engine/_2RealLink.h"
#include "helpers/_2RealEvent.h"
#include "helpers/_2RealPoco.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"
#include "helpers/_2RealNonCopyable.h"
//#include "_2RealSystemState.h"			// MOVE TO APP FOLDER

#include <set>
#include <string>
#include <list>

namespace _2Real
{

	namespace app
	{
		class Engine;
		//class SystemState;
	}

	class EngineImpl;

	class Logger;
	class Timer;
	class TypeRegistry;
	class ThreadPool;
	class BundleManager;
	class System;
	class LinkCollection;
	class Bundle;

	class LinkCollection
	{
	public:
		LinkCollection( EngineImpl * );
		~LinkCollection();
		void clear();
		std::shared_ptr< IOLink > createLink( std::shared_ptr< BasicInletIO >, std::shared_ptr< OutletIO > );
		/*std::shared_ptr< IOLink > createLink( std::shared_ptr< BasicInletIO >, std::shared_ptr< OutletIO > );*/
	private:
		struct LinkCmp
		{
			bool operator()( std::shared_ptr< IOLink > const& l1, std::shared_ptr< IOLink > const& l2 )
			{
				return ( *( l1.get() ) < *( l2.get() ) );
			}
		};
		typedef std::set< std::shared_ptr< IOLink >, LinkCmp >							Links;
		typedef std::set< std::shared_ptr< IOLink >, LinkCmp >::iterator				LinkIterator;
		typedef std::set< std::shared_ptr< IOLink >, LinkCmp >::const_iterator			LinkConstIterator;

		EngineImpl		*const mEngineImpl;
		Links			mLinks;
	};

	class EngineImpl : private NonCopyable< EngineImpl >
	{

	public:

		Logger *						getLogger()			{ return mLogger; }
		Timer *							getTimer()			{ return mTimer; }
		TypeRegistry *					getTypeRegistry()	{ return mTypeRegistry; }
		ThreadPool *					getThreadpool()		{ return mThreadPool; }
		BundleManager *					getBundleManager()	{ return mBundleManager; }
		System *						getBlockManager()	{ return mSystem; }
		LinkCollection *				getLinkManager()	{ return mLinkManager; }
		const long						getElapsedTime() const;

		void							clearFully();
		void							clearBlockInstances();

		std::shared_ptr< Bundle > 		loadLibrary( std::string const& path );
		std::string						getBundleDirectory() const;

		void							registerToException( app::BlockExcCallback &callback );
		void							unregisterFromException( app::BlockExcCallback &callback );
		void							registerToException( app::ContextBlockExcCallback &callback );
		void							unregisterFromException( app::ContextBlockExcCallback &callback );
		void							handleException( app::BlockHandle &block, Exception const& exception ) const;
		void							handleException( app::ContextBlockHandle &block, Exception const& exception ) const;

	private:

		friend class app::Engine;

		EngineImpl();
		~EngineImpl();

		// whatever you do. do not change the ordering of member variables here!
		// ( unless you absolutely have to, in which case, good luck )

		Logger					*const mLogger;
		Timer					*const mTimer;
		TypeRegistry			*const mTypeRegistry;
		ThreadPool				*const mThreadPool;
		BundleManager			*const mBundleManager;
		System					*const mSystem;
		LinkCollection			*const mLinkManager;

		Poco::Timestamp			mTimestamp;

		// TODO: exception handler class per block :)
		typedef std::pair< Exception, app::BlockHandle >			BlockException;
		typedef std::pair< Exception, app::ContextBlockHandle >		ContextBlockException;
		CallbackEvent< BlockException >								mBlockExceptionEvent;
		CallbackEvent< ContextBlockException >						mContextBlockExceptionEvent;

	};

}