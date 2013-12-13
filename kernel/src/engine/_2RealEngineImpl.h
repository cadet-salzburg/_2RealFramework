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

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealPath.h"

namespace _2Real
{
	namespace app
	{
		class Engine;
	}

	class EngineImpl;

	class TypeRegistry;
	class BundleCollection;
	class Bundle;

	/*
	class LinkCollection
	{
	public:
		LinkCollection( EngineImpl * );
		~LinkCollection();
		void clear();
		std::shared_ptr< IOLink >	createLink( std::shared_ptr< BasicInletIO >, std::shared_ptr< OutletIO > );
		void						destroyLink( std::shared_ptr< BasicInletIO >, std::shared_ptr< OutletIO > );
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

	class TimerCollection
	{
	public:
		TimerCollection( EngineImpl * );
		~TimerCollection();
		void clear();
		std::shared_ptr< Timer >	createTimer( const unsigned long resolution );
		void						destroyTimer( std::shared_ptr< Timer > );
		std::shared_ptr< Timer >	getDefaultTimer();
		std::shared_ptr< Timer >	getNullTimer();
	private:
		typedef std::set< std::shared_ptr< Timer > >									Timers;
		typedef std::set< std::shared_ptr< Timer > >::iterator							TimerIterator;
		typedef std::set< std::shared_ptr< Timer > >::const_iterator					TimerConstIterator;
		EngineImpl		*const mEngineImpl;
		Timers						mTimers;
		std::shared_ptr< Timer >	mDefaultTimer;
		std::shared_ptr< Timer >	mNullTimer;
	};
	*/

	class EngineImpl : public std::enable_shared_from_this< EngineImpl >
	{

	public:

		static std::shared_ptr< EngineImpl > create();

		~EngineImpl();

		std::shared_ptr< Bundle > 		loadLibrary( std::string const& filename );
		Path const&						getBundleDirectory() const;

	private:

		// sets up connections via objects that need it
		void privateInit();

		EngineImpl();
		EngineImpl( EngineImpl const& other );
		EngineImpl& operator=( EngineImpl const& other );

		// whatever you do. do not change the ordering of member variables here!
		// ( unless you absolutely have to, in which case, good luck )

		//Logger					*const mLogger;
		//Timer					*const mTimer;
		std::shared_ptr< TypeRegistry >		mTypeRegistry;
		//ThreadPool				*const mThreadPool;
		std::shared_ptr< BundleCollection >	mBundleCollection;
		//System					*const mSystem;
		//LinkCollection			*const mLinkManager;
		//TimerCollection			*const mTimerManager;

		//Poco::Timestamp			mTimestamp;

		// TODO: exception handler class per block :)
		//typedef std::pair< Exception, app::BlockHandle >				BlockException;
		//typedef std::pair< Exception, app::ContextBlockHandle >		ContextBlockException;
		//CallbackEvent< BlockException >								mBlockExceptionEvent;
		//CallbackEvent< ContextBlockException >						mContextBlockExceptionEvent;

	};

}