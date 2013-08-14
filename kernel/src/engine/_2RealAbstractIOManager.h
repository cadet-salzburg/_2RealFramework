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
	class AbstractUpdateManager;

	class TimestampedData;
	class AbstractInletBasedTrigger;

	class ADataCondition;

	class IOLink;
	class IOMetadata;

	class AbstractInletIO : private NonCopyable< AbstractInletIO >
	{

	public:

		virtual ~AbstractInletIO() {}

		virtual std::string const&						getFullHumanReadableName() const = 0;
		virtual std::string const&						getHumanReadableName() const = 0;
		//virtual std::string const&						getCode() const = 0;

		virtual bool									canExpand() const = 0;
		virtual unsigned int							getSubInletCount() const = 0;
		virtual std::shared_ptr< BasicInletIO >			operator[]( const unsigned int ) = 0;
		virtual std::shared_ptr< AbstractInletIO >		addSubInlet() = 0;
		virtual void									removeSubInlet( std::shared_ptr< AbstractInletIO > ) = 0;

		virtual void									synchronize() = 0;

		virtual std::shared_ptr< AbstractInlet >		getInlet() = 0;
		virtual std::shared_ptr< const AbstractInlet >	getInlet() const = 0;

		virtual std::shared_ptr< const IOMetadata >		getMetadata() const = 0;
		virtual bool									belongsToBlock( AbstractUberBlock const* ) const = 0;

	};

	class BasicInletIO : public AbstractInletIO
	{

	public:

		BasicInletIO( EngineImpl *, std::shared_ptr< InstanceId > , AbstractUberBlock *, std::shared_ptr< AbstractUpdateManager >, std::shared_ptr< const IOMetadata > );

		std::string const&							getFullHumanReadableName() const;
		std::string const&							getHumanReadableName() const;
		//std::string const&							getCode() const;

		void										setSelfRef( std::shared_ptr< BasicInletIO > ref );
		std::shared_ptr< BasicInletIO >				getSelfRef();
		std::shared_ptr< const BasicInletIO >		getSelfRef() const;

		bool										canExpand() const;
		unsigned int								getSubInletCount() const;
		std::shared_ptr< BasicInletIO >				operator[]( const unsigned int  );
		std::shared_ptr< AbstractInletIO >			addSubInlet();
		void										removeSubInlet( std::shared_ptr< AbstractInletIO > io );

		std::shared_ptr< AbstractInlet >			getInlet();
		std::shared_ptr< const AbstractInlet >		getInlet() const;

		std::shared_ptr< const IOMetadata >			getMetadata() const;
		bool										belongsToBlock( AbstractUberBlock const* ) const;

		bool										isBuffered() const;
		void										setQueueSize( const unsigned int size );

		void										setData( std::shared_ptr< const CustomType > );
		void										receiveData( std::shared_ptr< const CustomType > );
		void										receiveData( TimestampedData const& );
		std::shared_ptr< const CustomType >			getCurrentData() const;
		void										synchronize();

		bool										canLink() const;
		std::shared_ptr< IOLink >					linkTo( std::shared_ptr< OutletIO > );
		void										unlinkFrom( std::shared_ptr< OutletIO > );

		bool										canTriggerUpdates() const;
		bool										canFulfillCondition( std::shared_ptr< ADataCondition > ) const;
		bool										tryFulfillCondition( std::shared_ptr< ADataCondition > );

	private:

		EngineImpl									*const mEngineImpl;
		AbstractUberBlock							*const mOwningBlock;

		mutable Poco::FastMutex						mBufferAccess;
		std::shared_ptr< BasicInlet >				mInlet;
		std::shared_ptr< BasicInletBuffer >			mBuffer;
		std::shared_ptr< DataQueue >				mQueue;
		bool										mClearBuffer;

		std::weak_ptr< BasicInletIO >				mSelfRef;

		std::weak_ptr< AbstractUpdateManager >		mUpdateManager;
		
		std::shared_ptr< const IOMetadata >			mMetadata;
		std::shared_ptr< InstanceId > 		mIdentifier;

		TimestampedData								mLastData;

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

		MultiInletIO( EngineImpl *, std::shared_ptr< InstanceId > , AbstractUberBlock *, std::shared_ptr< AbstractUpdateManager >, std::shared_ptr< const IOMetadata > );

		std::string const&								getFullHumanReadableName() const;
		std::string const&								getHumanReadableName() const;
		//std::string const&								getCode() const;

		bool canExpand()								const { return true; }
		unsigned int getSubInletCount()					const { return mBasicInletIOs.size(); }
		std::shared_ptr< BasicInletIO >					operator[]( const unsigned int );
		std::shared_ptr< AbstractInletIO >				addSubInlet();
		void											removeSubInlet( std::shared_ptr< AbstractInletIO > );
		void											synchronize();

		std::shared_ptr< AbstractInlet >				getInlet();
		std::shared_ptr< const AbstractInlet >			getInlet() const;

		std::shared_ptr< const IOMetadata >				getMetadata() const;
		bool											belongsToBlock( AbstractUberBlock const* ) const;

	private:

		std::shared_ptr< MultiInlet >					mInlet;
		std::vector< IO >								mBasicInletIOs;
		std::list< std::shared_ptr< BasicInletIO > >	mTemporaryInletIOs;		// TODO: store outside of class
		mutable Poco::FastMutex							mAccess;

		EngineImpl										*const mEngineImpl;
		std::shared_ptr< AbstractUpdateManager >		mUpdateManager;
		AbstractUberBlock								*const mOwningBlock;
		std::shared_ptr< const IOMetadata >				mMetadata;
		std::shared_ptr< InstanceId > 		mIdentifier;

	};

	class OutletIO : private NonCopyable< OutletIO >
	{

	public:

		OutletIO( EngineImpl *, std::shared_ptr< InstanceId > , AbstractUberBlock *, std::shared_ptr< const IOMetadata > );
		~OutletIO();

		std::string const&						getFullHumanReadableName() const;
		std::string const&						getHumanReadableName() const;
		//std::string const&						getCode() const;

		void									setSelfRef( std::shared_ptr< OutletIO > ref ) { mSelfRef = ref; }
		std::shared_ptr< OutletIO >				getSelfRef();
		std::shared_ptr< const OutletIO >		getSelfRef() const;

		std::shared_ptr< Outlet >				getOutlet();
		std::shared_ptr< const Outlet >			getOutlet() const;

		std::shared_ptr< const IOMetadata >		getMetadata() const;
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
		std::shared_ptr< const IOMetadata >		mMetadata;
		std::shared_ptr< InstanceId >			mIdentifier;

		std::weak_ptr< OutletIO >				mSelfRef;

	public: // listener set / remove functions missing

		CallbackEvent< std::shared_ptr< const CustomType > >	*const mAppEvent;
		CallbackEvent< TimestampedData const& >					*const mInletEvent;

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
		typedef std::vector< std::shared_ptr< AbstractInletIO > >					ParameterVector;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::iterator			ParameterIterator;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::const_iterator	ParameterConstIterator;

		virtual ~AbstractIOManager() {}

		virtual std::string const&			getFullHumanReadableName() const = 0;
		virtual std::string const&			getHumanReadableName() const = 0;
		//virtual std::string const&			getCode() const = 0;

	};

}