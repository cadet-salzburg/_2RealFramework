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
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "engine/_2RealTimestampedData.h"
#include "helpers/_2RealException.h"

#include <list>
#include <vector>

namespace _2Real
{

	namespace bundle
	{
		class BlockHandle;
		class InletHandle;
		class OutletHandle;
	}

	namespace app
	{
		class AppData;
		class InletHandle;
		class OutletHandle;
	}

	class AbstractIOManager;
	class AbstractInlet;
	class AbstractInletBuffer;
	class BasicInlet;
	class BasicInletBuffer;
	class MultiInlet;
	class MultiInletBuffer;
	class BasicInletIO;
	class Outlet;
	class TimestampedData;
	class AbstractUberBlock;
	class AnyOptionSet;
	class TypeDescriptor;
	class AbstractUpdatePolicy;

	struct InletInfo
	{
		InletInfo( std::string const& n, InletPolicy const& p, std::shared_ptr< const CustomType > i, TypeMetadata const& t ) :
			baseName( n ), initializer( i ), type( t ), policy( p ) {}

		std::string								baseName;
		std::shared_ptr< const CustomType >		initializer;
		TypeMetadata							const& type;
		InletPolicy								policy;

	};

	class AbstractInletIO : private NonCopyable< AbstractInletIO >, private Handleable< AbstractInletIO, app::InletHandle >
	{

	public:

		using Handleable< AbstractInletIO, app::InletHandle >::getHandle;
		using Handleable< AbstractInletIO, app::InletHandle >::registerHandle;
		using Handleable< AbstractInletIO, app::InletHandle >::unregisterHandle;

		AbstractInletIO( AbstractUberBlock &owner, AbstractUpdatePolicy &policy, InletInfo const& info );
		virtual ~AbstractInletIO() {}

		virtual bool						isMultiInlet() const = 0;
		virtual unsigned int				getSize() const = 0;
		virtual BasicInletIO &				operator[]( const unsigned int index ) = 0;
		virtual AbstractInletIO *			addBasicInlet() = 0;
		virtual void						removeBasicInlet( AbstractInletIO *io ) = 0;
		virtual void						syncInletChanges() = 0;
		virtual bundle::InletHandle &		getBundleInletHandle() const = 0;

		InletInfo const&					info() const { return m_Info; }
		AbstractUberBlock *					getOwningBlock();

		bool								belongsToBlock( AbstractUberBlock const* const block ) const { return ( &m_OwningBlock == block ); }

	protected:

		InletInfo							m_Info;
		AbstractUpdatePolicy				&m_Policy;		// subinlets need to be +/- to the policy as well
		AbstractUberBlock					&m_OwningBlock;

	};

	class BasicInletIO : public AbstractInletIO
	{

	public:

		BasicInletIO( AbstractUberBlock &owner, AbstractUpdatePolicy &policy, InletInfo const& info );
		~BasicInletIO();

		bool								isMultiInlet() const { return false; }
		unsigned int						getSize() const { return 1; }
		BasicInletIO &						operator[]( const unsigned int index ) { return *this; }
		AbstractInletIO *					addBasicInlet() { throw Exception( "not a multiinlet" ); }
		void								removeBasicInlet( AbstractInletIO *io ){ throw Exception( "not a multiinlet" ); }
		void								syncInletChanges() {}
		bundle::InletHandle &				getBundleInletHandle() const;

		BasicInlet &						getInlet() { return *m_Inlet; }
		BasicInletBuffer &					getBuffer() { return *m_Buffer; }
		BasicInlet * getInletPtr()			{ return m_Inlet; }
		BasicInletBuffer * getBufferPtr()	{ return m_Buffer; }

		std::string const&					getName() const;
		void								setBufferSize( const unsigned int size );
		void								setUpdatePolicy( InletPolicy const& p );

		//void								receiveData( std::string const& dataAsString );

		void								receiveData( std::shared_ptr< const CustomType > data );
		void								syncInletData();
		void								processBufferedData( const bool enableTriggering );

		const std::string					getBufferSizeAsString() const;
		const std::string					getUpdatePolicyAsString() const;
		const std::string					getCurrentValueAsString() const;

		std::shared_ptr< const CustomType >		getCurrentData() const;

	private:

		BasicInlet							*m_Inlet;
		BasicInletBuffer					*m_Buffer;

	};

	class MultiInletIO : public AbstractInletIO
	{

	public:

		// holds inletios for all subinlets + update flags ( add or remove )
		struct IO
		{
			IO( BasicInletIO *io ) : io( io ), todoAdd( true ), todoRemove( false ) {}

			BasicInletIO	*io;
			bool			todoAdd;
			bool			todoRemove;
		};

		MultiInletIO( AbstractUberBlock &owner, AbstractUpdatePolicy &policy, InletInfo const& info );
		~MultiInletIO();

		bool isMultiInlet()					const { return true; }
		unsigned int getSize()				const { return m_InletIOs.size(); }
		BasicInletIO &						operator[]( const unsigned int index );
		AbstractInletIO *					addBasicInlet();
		void								removeBasicInlet( AbstractInletIO *io );
		void								syncInletChanges();
		bundle::InletHandle &				getBundleInletHandle() const;

	private:

		typedef std::vector< IO >						BasicIOs;
		typedef std::vector< IO >::iterator				BasicIOIterator;
		typedef std::vector< IO >::const_iterator		BasicIOConstInterator;

		BasicIOs							m_InletIOs;
		MultiInlet							*m_Inlet;
		MultiInletBuffer					*m_Buffer;
		mutable Poco::FastMutex				m_Access;

	};

	struct OutletInfo
	{
		OutletInfo( std::string const& n, std::shared_ptr< const CustomType > i, TypeMetadata const& t ) :
			baseName( n ), initializer( i ), type( t ) {}

		std::string								baseName;
		std::shared_ptr< const CustomType >		initializer;
		TypeMetadata							const& type;
	};

	class OutletIO : private NonCopyable< OutletIO >, private Handleable< OutletIO, app::OutletHandle >
	{

	public:

		using Handleable< OutletIO, app::OutletHandle >::getHandle;
		using Handleable< OutletIO, app::OutletHandle >::registerHandle;
		using Handleable< OutletIO, app::OutletHandle >::unregisterHandle;

		OutletIO( AbstractUberBlock &owner, OutletInfo const& info );
		~OutletIO();
		Outlet													*m_Outlet;
		CallbackEvent< app::AppData const& >					*m_AppEvent;
		CallbackEvent< TimestampedData const& >					*m_InletEvent;

		OutletInfo const&					info() const { return m_Info; }
		AbstractUberBlock *					getOwningBlock() { return &m_OwningBlock; }

		bool								belongsToBlock( AbstractUberBlock const* const block ) const { return ( &m_OwningBlock == block ); }

		std::shared_ptr< const CustomType >		getCurrentData() const;

		OutletInfo							m_Info;
		AbstractUberBlock					&m_OwningBlock;

	};

	typedef std::vector< app::InletHandle >							AppInletHandles;
	typedef std::vector< app::InletHandle >::iterator				AppInletHandleIterator;
	typedef std::vector< app::InletHandle >::const_iterator			AppInletHandleConstIterator;

	typedef std::vector< app::OutletHandle >						AppOutletHandles;
	typedef std::vector< app::OutletHandle >::iterator				AppOutletHandleIterator;
	typedef std::vector< app::OutletHandle >::const_iterator		AppOutletHandleConstIterator;

	typedef std::vector< bundle::InletHandle >						BundleInletHandles;
	typedef std::vector< bundle::InletHandle >::iterator			BundleInletHandleIterator;
	typedef std::vector< bundle::InletHandle >::const_iterator		BundleInletHandleConstIterator;

	typedef std::vector< bundle::OutletHandle >						BundleOutletHandles;
	typedef std::vector< bundle::OutletHandle >::iterator			BundleOutletHandleIterator;
	typedef std::vector< bundle::OutletHandle >::const_iterator		BundleOutletHandleConstIterator;

	class AbstractIOManager : private NonCopyable< AbstractIOManager >
	{

	public:

		typedef std::vector< AbstractInletIO * >					InletVector;
		typedef std::vector< AbstractInletIO * >::iterator			InletIterator;
		typedef std::vector< AbstractInletIO * >::const_iterator	InletConstIterator;

		typedef std::vector< OutletIO * >							OutletVector;
		typedef std::vector< OutletIO * >::iterator					OutletIterator;
		typedef std::vector< OutletIO * >::const_iterator			OutletConstIterator;

		AbstractIOManager( AbstractUberBlock &owner );
		virtual ~AbstractIOManager();
		const std::string getName() const;

	protected:

		AbstractUberBlock											&m_Owner;
		Poco::FastMutex												m_IOAccess;
		CallbackEvent< std::list< app::AppData > const& >			m_AppEvent;

	};

}