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

#include "_2RealAbstractUberBlock.h"
#include "_2RealAbstractIOManager.h"
#include "_2RealAbstractStateManager.h"
#include "_2RealUpdatePolicy.h"
#include "_2RealException.h"

namespace _2Real
{

	template< typename IOMgr, typename StateMgr >
	class UberBlock : public AbstractUberBlock
	{

	public:

		UberBlock( BlockIdentifier const& id );
		virtual ~UberBlock();

		void registerToNewData( std::string const& outlet, app::OutletDataCallback callback, void *userData );
		void unregisterFromNewData( std::string const& outlet, app::OutletDataCallback callback, void *userData );
		void registerToNewData( std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler );
		void unregisterFromNewData( std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler );
		void registerToNewData( app::BlockDataCallback callback, void *userData );
		void unregisterFromNewData( app::BlockDataCallback callback, void *userData );
		void registerToNewData( app::AbstractBlockDataCallbackHandler &handler );
		void unregisterFromNewData( app::AbstractBlockDataCallbackHandler &handler );

		void setUp();
		void start();
		void setUpdatePolicy( UpdatePolicy const& policy );
		void stop( const bool blocking, const long timeout );
		void prepareForShutDown();
		bool shutDown( const long timeout );

		Inlet & getInlet( std::string const& name );
		Outlet & getOutlet( std::string const& name );
		Parameter & getParameter( std::string const& name );

		app::UpdatePolicyHandle getUpdatePolicyHandle() const;

		virtual void		createLink( Inlet &inlet, Outlet &outlet ) = 0;
		virtual void		destroyLink( Inlet &inlet, Outlet &outlet ) = 0;

	protected:

		AbstractStateManager &	getStateManager();
		AbstractIOManager &		getIOManager();

		IOMgr					*m_IOManager;
		StateMgr				*m_StateManager;
		UpdatePolicy			*m_UpdatePolicy;

	};

	template< typename IOMgr, typename StateMgr >
	UberBlock< IOMgr, StateMgr >::UberBlock( BlockIdentifier const& id ) :
		AbstractUberBlock( id ),
		m_IOManager( new IOMgr( *this ) ),
		m_StateManager( new StateMgr( *this ) ),
		m_UpdatePolicy( new UpdatePolicy( *this, *m_StateManager, *m_IOManager ) )
	{
	}

	template< typename IOMgr, typename StateMgr >
	UberBlock< IOMgr, StateMgr >::~UberBlock()
	{
		//delete m_UpdatePolicy;
		//delete m_StateManager;
		//delete m_IOManager;
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::registerToNewData(std::string const& outlet, app::OutletDataCallback callback, void *userData)
	{
		m_IOManager->registerToNewData(outlet, callback, userData);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::unregisterFromNewData(std::string const& outlet, app::OutletDataCallback callback, void *userData)
	{
		m_IOManager->unregisterFromNewData(outlet, callback, userData);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::registerToNewData(std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler)
	{
		m_IOManager->registerToNewData(outlet, handler);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::unregisterFromNewData(std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler)
	{
		m_IOManager->unregisterFromNewData(outlet, handler);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::registerToNewData(app::BlockDataCallback callback, void *userData)
	{
		m_IOManager->registerToNewData(callback, userData);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::unregisterFromNewData(app::BlockDataCallback callback, void *userData)
	{
		m_IOManager->unregisterFromNewData(callback, userData);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::registerToNewData(app::AbstractBlockDataCallbackHandler &handler)
	{
		m_IOManager->registerToNewData(handler);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::unregisterFromNewData(app::AbstractBlockDataCallbackHandler &handler)
	{
		m_IOManager->unregisterFromNewData(handler);
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::setUp()
	{
		m_StateManager->setUp();
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::start()
	{
		m_StateManager->start();
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::stop( const bool blocking, const long timeout )
	{
		Poco::Event & ev = m_StateManager->stop();
		if ( blocking )
		{
			if ( !ev.tryWait( timeout ) )
			{
				std::ostringstream msg;
				msg << "timeout reached on " << m_Identifier.getName() << " stop()" << std::endl;
				throw TimeOutException( msg.str() );
			}
		}
	}

	template< typename IOMgr, typename StateMgr >
	void UberBlock< IOMgr, StateMgr >::prepareForShutDown()
	{
		m_StateManager->prepareForShutDown();
	}

	template< typename IOMgr, typename StateMgr >
	bool UberBlock< IOMgr, StateMgr >::shutDown( const long timeout )
	{
		return m_StateManager->shutDown( timeout );
	}

	template< typename IOMgr, typename StateMgr >
	Inlet & UberBlock< IOMgr, StateMgr >::getInlet( std::string const& name )
	{
		return m_IOManager->getInlet( name );
	}

	template< typename IOMgr, typename StateMgr >
	Outlet & UberBlock< IOMgr, StateMgr >::getOutlet( std::string const& name )
	{
		return m_IOManager->getOutlet( name );
	}

	template< typename IOMgr, typename StateMgr >
	Parameter & UberBlock< IOMgr, StateMgr >::getParameter( std::string const& name )
	{
		return m_IOManager->getParameter( name );
	}

	template< typename IOMgr, typename StateMgr >
	app::UpdatePolicyHandle UberBlock< IOMgr, StateMgr >::getUpdatePolicyHandle() const
	{
		return app::UpdatePolicyHandle( *m_UpdatePolicy );
	}

	template< typename IOMgr, typename StateMgr >
	AbstractStateManager & UberBlock< IOMgr, StateMgr >::getStateManager()
	{
		return *m_StateManager;
	}

	template< typename IOMgr, typename StateMgr >
	AbstractIOManager & UberBlock< IOMgr, StateMgr >::getIOManager()
	{
		return *m_IOManager;
	}

}