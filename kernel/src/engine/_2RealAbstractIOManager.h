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

#include "helpers/_2RealEvent.h"
#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealParameterHandle.h"
#include "engine/_2RealTimestampedData.h"
#include "helpers/_2RealException.h"
#include "policies/_2RealUpdatePolicy.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	// WARNING: this file is a mess ;)
	// after some changes @july 13, i believe inlets / outlets / parameters might now refactored & the code might get a lot shorter;
	// however, I don't have time for this atm

	class EngineImpl;
	class AbstractIOManager;

	class AbstractInlet;
	class AbstractInletIO;
	class BasicInlet;
	class BasicInletIO;
	class BasicInletBuffer;
	class MultiInlet;
	class MultiInletIO;
	class DataQueue;

	class Outlet;
	class OutletBuffer;
	class OutletIO;

	class Parameter;
	class ParameterBuffer;
	class ParameterIO;

	class AbstractUberBlock;
	class AbstractUpdatePolicy;

	class TimestampedData;
	class AbstractInletBasedTrigger;

	class IOLink;
	class IOMetadata;

	class AbstractInletIO : private NonCopyable< AbstractInletIO >, private Identifiable< AbstractInletIO >
	{

	public:

		using Identifiable< AbstractInletIO >::getFullName;
		using Identifiable< AbstractInletIO >::getName;

		AbstractInletIO( EngineImpl *, AbstractUberBlock *, AbstractUpdatePolicy *, std::shared_ptr< const IOMetadata > );
		virtual ~AbstractInletIO() {}

		virtual bool									canExpand() const = 0;
		virtual unsigned int							getSubInletCount() const = 0;
		virtual std::shared_ptr< BasicInletIO >			operator[]( const unsigned int ) = 0;
		virtual std::shared_ptr< AbstractInletIO >		addSubInlet() = 0;
		virtual void									removeSubInlet( std::shared_ptr< AbstractInletIO > ) = 0;

		virtual void									synchronizeData() = 0;

		virtual std::shared_ptr< AbstractInlet >		getInlet() = 0;
		virtual std::shared_ptr< const AbstractInlet >	getInlet() const = 0;

		std::shared_ptr< const IOMetadata >				getInfo() const;
		bool											belongsToBlock( AbstractUberBlock const* ) const;

	protected:

		EngineImpl										*const mEngineImpl;
		AbstractUpdatePolicy							*const mPolicy;			// subinlets need to be +/- to the policy as well
		AbstractUberBlock								*const mOwningBlock;
		std::shared_ptr< const IOMetadata >				mMetadata;

	};

	class BasicInletIO : public AbstractInletIO
	{

	public:

		BasicInletIO( EngineImpl *, AbstractUberBlock *, AbstractUpdatePolicy *, std::shared_ptr< const IOMetadata > );

		void										setSelfRef( std::shared_ptr< BasicInletIO > ref );
		std::shared_ptr< BasicInletIO >				getSelfRef();
		std::shared_ptr< const BasicInletIO >		getSelfRef() const;

		bool										canExpand() const;
		unsigned int								getSubInletCount() const;
		std::shared_ptr< BasicInletIO >				operator[]( const unsigned int  );
		std::shared_ptr< AbstractInletIO >			addSubInlet();
		void										removeSubInlet( std::shared_ptr< AbstractInletIO > io );
		void										syncInletChanges();

		std::shared_ptr< AbstractInlet >			getInlet();
		std::shared_ptr< const AbstractInlet >		getInlet() const;

		bool										isBuffered() const;
		void										setQueueSize( const unsigned int size );
		void										processQueue();

		void										setData( std::shared_ptr< const CustomType > );
		void										receiveData( std::shared_ptr< const CustomType > );
		void										receiveData( TimestampedData const& );
		std::shared_ptr< const CustomType >			getCurrentData() const;
		void										synchronizeData();

		bool										canLink() const;
		std::shared_ptr< IOLink >					linkTo( std::shared_ptr< OutletIO > );
		void										unlinkFrom( std::shared_ptr< OutletIO > );

		bool										canTriggerUpdates() const;
		void										setUpdatePolicy( UpdatePolicy const& );
		void										setTrigger( AbstractInletBasedTrigger * );
		void										removeTrigger( AbstractInletBasedTrigger * );

	private:

		std::shared_ptr< BasicInlet >			mInlet;
		std::shared_ptr< BasicInletBuffer >		mBuffer;
		std::shared_ptr< DataQueue >			mQueue;

		std::weak_ptr< BasicInletIO >			mSelfRef;

		mutable Poco::FastMutex					mAccess;
		AbstractInletBasedTrigger				*mUpdateTrigger;
		bool									mNotifyOnReceive;

	};

	class MultiInletIO : public AbstractInletIO
	{

	public:

		// holds inletios for all subinlets + update flags ( remove y/n )
		struct IO
		{
			IO( std::shared_ptr< BasicInletIO > io ) : io( io ), wasRemoved( false ) {}

			std::shared_ptr< BasicInletIO > 	io;
			bool								wasRemoved;
		};

		MultiInletIO( EngineImpl *, AbstractUberBlock *, AbstractUpdatePolicy *, std::shared_ptr< const IOMetadata > );
		~MultiInletIO();

		bool canExpand()								const { return true; }
		unsigned int getSubInletCount()					const { return mBasicInletIOs.size(); }
		std::shared_ptr< BasicInletIO >					operator[]( const unsigned int );
		std::shared_ptr< AbstractInletIO >				addSubInlet();
		void											removeSubInlet( std::shared_ptr< AbstractInletIO > );
		void											synchronizeData();

		std::shared_ptr< AbstractInlet >				getInlet();
		std::shared_ptr< const AbstractInlet >			getInlet() const;

	private:

		std::shared_ptr< MultiInlet >					mInlet;
		std::vector< IO >								mBasicInletIOs;
		std::list< std::shared_ptr< BasicInletIO > >	mTemporaryInletIOs;		// TODO: store outside of class
		mutable Poco::FastMutex							mAccess;

	};

	class OutletIO : private NonCopyable< OutletIO >, private Identifiable< OutletIO >
	{

	public:

		using Identifiable< OutletIO >::getFullName;
		using Identifiable< OutletIO >::getName;

		OutletIO( EngineImpl *, AbstractUberBlock *, std::shared_ptr< const IOMetadata > );
		~OutletIO();

		void									setSelfRef( std::shared_ptr< OutletIO > ref ) { mSelfRef = ref; }
		std::shared_ptr< OutletIO >				getSelfRef();
		std::shared_ptr< const OutletIO >		getSelfRef() const;

		std::shared_ptr< Outlet >				getOutlet();
		std::shared_ptr< const Outlet >			getOutlet() const;

		std::shared_ptr< const IOMetadata >		getInfo() const;
		bool									belongsToBlock( AbstractUberBlock const* ) const;

		std::shared_ptr< const CustomType >		getCurrentDataThreadsafe() const;
		std::shared_ptr< const CustomType >		synchronizeData();

		std::shared_ptr< IOLink >				linkTo( std::shared_ptr< BasicInletIO > );
		void									unlinkFrom( std::shared_ptr< BasicInletIO > );

	private:

		EngineImpl								*const mEngineImpl;
		AbstractUberBlock						*const mOwningBlock;
		std::shared_ptr< Outlet >				mOutlet;
		OutletBuffer							*const mBuffer;
		std::shared_ptr< const IOMetadata >		mInfo;

		std::weak_ptr< OutletIO >				mSelfRef;

	public: // listener set / remove functions missing

		CallbackEvent< std::shared_ptr< const CustomType > >	*const mAppEvent;
		CallbackEvent< TimestampedData const& >					*const mInletEvent;

	};

	class ParameterIO : private NonCopyable< ParameterIO >, private Identifiable< ParameterIO >
	{

	public:

		using Identifiable< ParameterIO >::getFullName;
		using Identifiable< ParameterIO >::getName;

		ParameterIO( EngineImpl *, AbstractUberBlock *, std::shared_ptr< const IOMetadata > );
		~ParameterIO();

		std::shared_ptr< const IOMetadata >		getInfo() const;
		bool									belongsToBlock( AbstractUberBlock const* ) const;

		std::shared_ptr< Parameter >			getParameter();
		std::shared_ptr< const Parameter >		getParameter() const;

		std::shared_ptr< const CustomType >		getCurrentDataThreadsafe() const;
		void									synchronizeData();
		void									setData( std::shared_ptr< const CustomType > );

	private:

		EngineImpl								*const mEngineImpl;
		AbstractUberBlock						*const mOwningBlock;
		std::shared_ptr< Parameter >			mParameter;
		ParameterBuffer							*const mBuffer;
		std::shared_ptr< const IOMetadata >		mInfo;

	};

	class AbstractIOManager : private NonCopyable< AbstractIOManager >
	{

	public:

		typedef std::vector< std::shared_ptr< AbstractInletIO > >					InletVector;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::iterator			InletIterator;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::const_iterator	InletConstIterator;

		typedef std::vector< std::shared_ptr< OutletIO > >							OutletVector;
		typedef std::vector< std::shared_ptr< OutletIO > >::iterator				OutletIterator;
		typedef std::vector< std::shared_ptr< OutletIO > >::const_iterator			OutletConstIterator;

		typedef std::vector< std::shared_ptr< ParameterIO > >						ParameterVector;
		typedef std::vector< std::shared_ptr< ParameterIO > >::iterator				ParameterIterator;
		typedef std::vector< std::shared_ptr< ParameterIO > >::const_iterator		ParameterConstIterator;


		AbstractIOManager( EngineImpl *, AbstractUberBlock * );
		virtual ~AbstractIOManager();

		std::string						getName() const;		// shortcut for owner's name

	protected:

		EngineImpl						*const mEngineImpl;
		AbstractUberBlock				*const mOwner;

	};

}