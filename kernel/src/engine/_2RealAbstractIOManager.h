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
#include "helpers/_2RealPoco.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealIdentifiable.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealParameterHandle.h"
#include "engine/_2RealTimestampedData.h"
#include "helpers/_2RealException.h"

#include <list>
#include <vector>
#include <string>

namespace _2Real
{

	// WARNING: this file is a mess ;)
	// after some changes @july 13, i believe inlets / outlets / parameters might now refactored & the code might get a lot shorter;
	// however, I don't have time for this atm

	namespace bundle
	{
		class BlockHandle;
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
	}

	namespace app
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;
	}

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

	struct IOInfo
	{
		IOInfo( std::string const& n, std::shared_ptr< const CustomType > i, TypeMetadata const& t, Policy const& p ) :
			name( n ), initializer( i ), type( t ), policy( p ) {}

		std::string								name;
		std::shared_ptr< const CustomType >		initializer;
		TypeMetadata							const& type;
		Policy									policy;

	};

	class AbstractInletIO : private NonCopyable< AbstractInletIO >, private Identifiable< AbstractInletIO >, private Handleable< AbstractInletIO, app::InletHandle >
	{

	public:

		using Handleable< AbstractInletIO, app::InletHandle >::getHandle;
		using Handleable< AbstractInletIO, app::InletHandle >::registerHandle;
		using Handleable< AbstractInletIO, app::InletHandle >::unregisterHandle;

		using Identifiable< AbstractInletIO >::getFullName;
		using Identifiable< AbstractInletIO >::getName;

		AbstractInletIO( EngineImpl *, AbstractUberBlock *, AbstractUpdatePolicy *, IOInfo * );
		virtual ~AbstractInletIO() {}

		virtual bool						isMultiInlet() const = 0;
		virtual unsigned int				getSize() const = 0;
		virtual BasicInletIO &				operator[]( const unsigned int ) = 0;
		virtual AbstractInletIO *			addBasicInlet() = 0;
		virtual void						removeBasicInlet( AbstractInletIO * ) = 0;

		virtual void						synchronizeData() = 0;
		virtual bundle::InletHandle &		getBundleHandle() const = 0;

		IOInfo *							getInfo();
		AbstractUberBlock *					getOwningBlock();
		bool								belongsToBlock( AbstractUberBlock const* ) const;

	protected:

		EngineImpl							*const mEngineImpl;
		AbstractUpdatePolicy				*const mPolicy;			// subinlets need to be +/- to the policy as well
		AbstractUberBlock					*const mOwningBlock;
		IOInfo								*const mInfo;

	};

	class BasicInletIO : public AbstractInletIO
	{

	public:

		BasicInletIO( EngineImpl *, AbstractUberBlock *, AbstractUpdatePolicy *, IOInfo * );
		~BasicInletIO();

		bool								isMultiInlet() const { return false; }
		unsigned int						getSize() const { return 1; }
		BasicInletIO &						operator[]( const unsigned int index ) { return *this; }
		AbstractInletIO *					addBasicInlet() { throw Exception( "not a multiinlet" ); }
		void								removeBasicInlet( AbstractInletIO *io ){ throw Exception( "not a multiinlet" ); }
		void								syncInletChanges() {}

		bundle::InletHandle &				getBundleHandle() const;
		void								synchronizeData();

		BasicInlet &						getInlet() { return *mInlet; }

		void								setQueueSize( const unsigned int size );
		void								setUpdatePolicy( Policy const& p );

		void								setData( std::shared_ptr< const CustomType > );		// set buffer data ( b/c rec would trigger an update )
		void								receiveData( std::shared_ptr< const CustomType > );
		void								receiveData( TimestampedData const& );
		void								processQueue();

		const std::string					getQueueSizeAsString() const;
		const std::string					getUpdatePolicyAsString() const;
		const std::string					getCurrentDataAsString() const;

		std::shared_ptr< const CustomType >	getCurrentData() const;

		bool								linkTo( OutletIO * );
		void								unlinkFrom( OutletIO * );

		void								setTrigger( AbstractInletBasedTrigger * );
		void								removeTrigger( AbstractInletBasedTrigger * );

	private:

		BasicInlet							*const mInlet;
		BasicInletBuffer					*const mBuffer;
		DataQueue							*const mQueue;

		mutable Poco::FastMutex				mAccess;
		AbstractInletBasedTrigger			*mUpdateTrigger;
		bool								mNotifyOnReceive;

	};

	class MultiInletIO : public AbstractInletIO
	{

	public:

		// holds inletios for all subinlets + update flags ( remove y/n )
		struct IO
		{
			IO( BasicInletIO *io ) : io( io ), wasRemoved( false ) {}

			BasicInletIO	*io;
			bool			wasRemoved;
		};

		MultiInletIO( EngineImpl *, AbstractUberBlock *, AbstractUpdatePolicy *, IOInfo * );
		~MultiInletIO();

		bool isMultiInlet()						const { return true; }
		unsigned int getSize()					const { return mBasicInletIOs.size(); }
		BasicInletIO &							operator[]( const unsigned int index );
		AbstractInletIO *						addBasicInlet();
		void									removeBasicInlet( AbstractInletIO *io );
		void									synchronizeData();
		bundle::InletHandle &					getBundleHandle() const;

	private:

		MultiInlet								*const mInlet;
		std::vector< IO >						mBasicInletIOs;
		std::list< BasicInletIO * >				mTemporaryInletIOs;		// store outside?
		mutable Poco::FastMutex					mAccess;

	};

	class OutletIO : private NonCopyable< OutletIO >, private Identifiable< OutletIO >, private Handleable< OutletIO, app::OutletHandle >
	{

	public:

		using Handleable< OutletIO, app::OutletHandle >::getHandle;
		using Handleable< OutletIO, app::OutletHandle >::registerHandle;
		using Handleable< OutletIO, app::OutletHandle >::unregisterHandle;

		using Identifiable< OutletIO >::getFullName;
		using Identifiable< OutletIO >::getName;

		OutletIO( EngineImpl *, AbstractUberBlock *, IOInfo * );
		~OutletIO();

		IOInfo *								getInfo();
		AbstractUberBlock *						getOwningBlock();
		bool									belongsToBlock( AbstractUberBlock const* ) const;

		std::shared_ptr< const CustomType >		getCurrentDataThreadsafe() const;
		std::shared_ptr< const CustomType >		synchronizeData();

		bool									linkTo( BasicInletIO & );
		void									unlinkFrom( BasicInletIO & );

		bundle::OutletHandle					getBundleHandle() const;

	private:

		EngineImpl								*const mEngineImpl;
		AbstractUberBlock						*const mOwningBlock;
		Outlet									*const mOutlet;
		OutletBuffer							*const mBuffer;
		IOInfo									*const mInfo;

	public: // listener set / remove functions missing!!!!

		CallbackEvent< std::shared_ptr< const CustomType > >	*const mAppEvent;
		CallbackEvent< TimestampedData const& >					*const mInletEvent;

	};

	class ParameterIO : private NonCopyable< ParameterIO >, private Identifiable< ParameterIO >, private Handleable< ParameterIO, app::ParameterHandle >
	{

	public:

		using Handleable< ParameterIO, app::ParameterHandle >::getHandle;
		using Handleable< ParameterIO, app::ParameterHandle >::registerHandle;
		using Handleable< ParameterIO, app::ParameterHandle >::unregisterHandle;

		using Identifiable< ParameterIO >::getFullName;
		using Identifiable< ParameterIO >::getName;

		ParameterIO( EngineImpl *, AbstractUberBlock *, IOInfo * );
		~ParameterIO();

		IOInfo *								getInfo();
		AbstractUberBlock *						getOwningBlock();
		bool									belongsToBlock( AbstractUberBlock const* ) const;

		std::shared_ptr< const CustomType >		getCurrentDataThreadsafe() const;
		void									synchronizeData();
		void									setData( std::shared_ptr< const CustomType > );

		bundle::ParameterHandle					getBundleHandle() const;

	private:

		EngineImpl								*const mEngineImpl;
		AbstractUberBlock						*const mOwningBlock;
		Parameter								*const mParameter;
		ParameterBuffer							*const mBuffer;
		IOInfo									*const mInfo;

	};

	typedef std::vector< app::InletHandle >							AppInletHandles;
	typedef std::vector< app::InletHandle >::iterator				AppInletHandleIterator;
	typedef std::vector< app::InletHandle >::const_iterator			AppInletHandleConstIterator;

	typedef std::vector< app::OutletHandle >						AppOutletHandles;
	typedef std::vector< app::OutletHandle >::iterator				AppOutletHandleIterator;
	typedef std::vector< app::OutletHandle >::const_iterator		AppOutletHandleConstIterator;

	typedef std::vector< app::ParameterHandle >						AppParameterHandles;
	typedef std::vector< app::ParameterHandle >::iterator			AppParameterHandleIterator;
	typedef std::vector< app::ParameterHandle >::const_iterator		AppParameterHandleConstIterator;

	typedef std::vector< bundle::InletHandle >						BundleInletHandles;
	typedef std::vector< bundle::InletHandle >::iterator			BundleInletHandleIterator;
	typedef std::vector< bundle::InletHandle >::const_iterator		BundleInletHandleConstIterator;

	typedef std::vector< bundle::OutletHandle >						BundleOutletHandles;
	typedef std::vector< bundle::OutletHandle >::iterator			BundleOutletHandleIterator;
	typedef std::vector< bundle::OutletHandle >::const_iterator		BundleOutletHandleConstIterator;

	typedef std::vector< bundle::ParameterHandle >					BundleParameterHandles;
	typedef std::vector< bundle::ParameterHandle >::iterator		BundleParameterHandleIterator;
	typedef std::vector< bundle::ParameterHandle >::const_iterator	BundleParameterHandleConstIterator;

	class AbstractIOManager : private NonCopyable< AbstractIOManager >
	{

	public:

		typedef std::vector< AbstractInletIO * >					InletVector;
		typedef std::vector< AbstractInletIO * >::iterator			InletIterator;
		typedef std::vector< AbstractInletIO * >::const_iterator	InletConstIterator;

		typedef std::vector< OutletIO * >							OutletVector;
		typedef std::vector< OutletIO * >::iterator					OutletIterator;
		typedef std::vector< OutletIO * >::const_iterator			OutletConstIterator;

		typedef std::vector< ParameterIO * >						ParameterVector;
		typedef std::vector< ParameterIO * >::iterator				ParameterIterator;
		typedef std::vector< ParameterIO * >::const_iterator		ParameterConstIterator;


		AbstractIOManager( EngineImpl *, AbstractUberBlock * );
		virtual ~AbstractIOManager();

		std::string						getName() const;

	protected:

		EngineImpl						*const mEngineImpl;
		AbstractUberBlock				*const mOwner;

	};

}