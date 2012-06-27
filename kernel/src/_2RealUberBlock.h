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
#include "_2RealAbstractBlockManager.h"
#include "_2RealAbstractIOManager.h"
#include "_2RealAbstractStateManager.h"
#include "_2RealException.h"
#include "_2RealUpdatePolicyl.h"

namespace _2Real
{

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	class UberBlock : public AbstractUberBlock
	{

	public:

		UberBlock( BlockIdentifier const& id );
		virtual ~UberBlock();

		void addSuperBlock( AbstractUberBlock &superBlock, AbstractUberBlockBasedTrigger *trigger );
		void removeSuperBlock( AbstractUberBlock &superBlock );
		void addSubBlock( AbstractUberBlock &subBlock, AbstractUberBlockBasedTrigger *trigger );
		void removeSubBlock( AbstractUberBlock &subBlock );

		void registerToNewData( std::string const& outlet, OutletCallback callback, void *userData );
		void unregisterFromNewData( std::string const& outlet, OutletCallback callback, void *userData );
		void registerToNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler );
		void unregisterFromNewData( std::string const& outlet, AbstractOutletCallbackHandler &handler );

		void registerToNewData( OutputCallback callback, void *userData );
		void unregisterFromNewData( OutputCallback callback, void *userData );
		void registerToNewData( AbstractOutputCallbackHandler &handler );
		void unregisterFromNewData( AbstractOutputCallbackHandler &handler );
		EngineData const& getValue( std::string const& paramName ) const;
		std::string const& getTypename( std::string const& paramName ) const;
		std::string const& getLongTypename( std::string const& paramName ) const;
		void setValue( std::string const& paramName, TimestampedData const& value );

		void setUp();
		void start();
		void setUpdatePolicy( UpdatePolicy const& policy );
		void stop( const bool blocking, const long timeout );
		void prepareForShutDown();
		bool shutDown( const long timeout );

		Inlet & getInlet( std::string const& name );
		Outlet & getOutlet( std::string const& name );
		SetupParameter & getSetupParameter( std::string const& name );

		UpdatePolicyHandle getUpdatePolicyHandle() const;

	protected:

		AbstractIOManager				*m_IOManager;
		AbstractBlockManager			*m_SuperBlockManager;
		AbstractBlockManager			*m_SubBlockManager;
		AbstractStateManager			*m_StateManager;
		UpdatePolicy					*m_UpdatePolicy;

	};

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::UberBlock( BlockIdentifier const& id ) :
		AbstractUberBlock( id ),
		m_IOManager( new IOMgr( *this ) ),
		m_SuperBlockManager( new SuperBlockMgr( *this ) ),
		m_SubBlockManager( new SubBlockMgr( *this ) ),
		m_StateManager( new StateMgr( *this ) ),
		m_UpdatePolicy( new UpdatePolicy( *this, *m_StateManager, *m_IOManager ) )
	{
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::~UberBlock()
	{
		//delete m_UpdatePolicy;
		//delete m_StateManager;
		//delete m_IOManager;
		//delete m_SubBlockManager;
		//delete m_SuperBlockManager;
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::addSuperBlock( AbstractUberBlock &superBlock, AbstractUberBlockBasedTrigger *trigger )
	{
		m_SuperBlockManager->addBlock( superBlock );
		//if ( trigger != nullptr )
		//{
		//	m_StateManager->addTriggerForSuperBlock( superBlock.getId(), *trigger );
		//}
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::removeSuperBlock( AbstractUberBlock &superBlock )
	{
		m_SuperBlockManager->removeBlock( superBlock );
		//m_StateManager->removeTriggerForSuperBlock( superBlock.getId() );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::addSubBlock( AbstractUberBlock &subBlock, AbstractUberBlockBasedTrigger *trigger )
	{
		m_SubBlockManager->addBlock( subBlock );
		//if ( trigger != nullptr )
		//{
		//	m_StateManager->addTriggerForSubBlock( subBlock.getId(), *trigger );
		//}
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::removeSubBlock( AbstractUberBlock &subBlock )
	{
		m_SubBlockManager->removeBlock( subBlock );
		//m_StateManager->removeTriggerForSubBlock( subBlock.getId() );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::registerToNewData(std::string const& outlet, OutletCallback callback, void *userData)
	{
		m_IOManager->registerToNewData(outlet, callback, userData);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::unregisterFromNewData(std::string const& outlet, OutletCallback callback, void *userData)
	{
		m_IOManager->unregisterFromNewData(outlet, callback, userData);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::registerToNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler)
	{
		m_IOManager->registerToNewData(outlet, handler);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::unregisterFromNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler)
	{
		m_IOManager->unregisterFromNewData(outlet, handler);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::registerToNewData(OutputCallback callback, void *userData)
	{
		m_IOManager->registerToNewData(callback, userData);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::unregisterFromNewData(OutputCallback callback, void *userData)
	{
		m_IOManager->unregisterFromNewData(callback, userData);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::registerToNewData(AbstractOutputCallbackHandler &handler)
	{
		m_IOManager->registerToNewData(handler);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::unregisterFromNewData(AbstractOutputCallbackHandler &handler)
	{
		m_IOManager->unregisterFromNewData(handler);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	EngineData const& UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::getValue(std::string const& paramName) const
	{
		return m_IOManager->getValue(paramName);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	std::string const& UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::getTypename( std::string const& paramName ) const
	{
		return m_IOManager->getTypename( paramName );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	std::string const& UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::getLongTypename( std::string const& paramName ) const
	{
		return m_IOManager->getLongTypename( paramName );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::setValue(std::string const& paramName, TimestampedData const& value)
	{
		m_IOManager->setValue(paramName, value);
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::setUp()
	{
		m_StateManager->setUp();
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::start()
	{
		m_StateManager->start();
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::stop( const bool blocking, const long timeout )
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

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	void UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::prepareForShutDown()
	{
		m_StateManager->prepareForShutDown();
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	bool UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::shutDown( const long timeout )
	{
		return m_StateManager->shutDown( timeout );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	Inlet & UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::getInlet( std::string const& name )
	{
		return m_IOManager->getInlet( name );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	Outlet & UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::getOutlet( std::string const& name )
	{
		return m_IOManager->getOutlet( name );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	SetupParameter & UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::getSetupParameter( std::string const& name )
	{
		return m_IOManager->getSetupParameter( name );
	}

	template< typename IOMgr, typename SuperBlockMgr, typename SubBlockMgr, typename StateMgr >
	UpdatePolicyHandle UberBlock< IOMgr, SuperBlockMgr, SubBlockMgr, StateMgr >::getUpdatePolicyHandle() const
	{
		return UpdatePolicyHandle( *m_UpdatePolicy );
	}

}