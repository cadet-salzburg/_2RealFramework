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
#include "_2RealAbstractUberBlockManager.h"
#include "_2RealAbstractIOManager.h"
#include "_2RealAbstractStateManager.h"
#include "_2RealException.h"

namespace _2Real
{

	template< typename IOMgr, typename UberBlockMgr, typename SubBlockMgr, typename StateMgr >
	class UberBlock : public AbstractUberBlock
	{

	public:

		UberBlock( BlockIdentifier const& id ) :
			AbstractUberBlock( id ),
			m_IOManager( new IOMgr( *this ) ),
			m_SubBlockManager( new SubBlockMgr( *this ) ),
			m_SuperBlockManager( new UberBlockMgr( *this ) ),
			m_StateManager( new StateMgr( *this ) )
		{
		}

		virtual ~UberBlock()
		{
			delete m_StateManager;
			delete m_IOManager;
			delete m_SubBlockManager;
			delete m_SuperBlockManager;
		}

		void addSuperBlock( AbstractUberBlock &superBlock, AbstractUberBlockBasedTrigger *trigger )
		{
			m_SuperBlockManager->addBlock( superBlock );
			if ( trigger != nullptr )
			{
				m_StateManager->addTriggerForSuperBlock( superBlock.getId(), *trigger );
			}
		}
		
		void removeSuperBlock( AbstractUberBlock &superBlock )
		{
			m_SuperBlockManager->removeBlock( superBlock );
			m_StateManager->removeTriggerForSuperBlock( superBlock.getId() );
		}

		void addSubBlock( AbstractUberBlock &subBlock, AbstractUberBlockBasedTrigger *trigger )
		{
			m_SubBlockManager->addBlock( subBlock );
			if ( trigger != nullptr )
			{
				m_StateManager->addTriggerForSubBlock( subBlock.getId(), *trigger );
			}
		}

		void removeSubBlock( AbstractUberBlock &subBlock )
		{
			m_SubBlockManager->removeBlock( subBlock );
			m_StateManager->removeTriggerForSubBlock( subBlock.getId() );
		}

		void registerToNewData(std::string const& outlet, OutletCallback callback, void *userData)
		{
			m_IOManager->registerToNewData(outlet, callback, userData);
		}

		void unregisterFromNewData(std::string const& outlet, OutletCallback callback, void *userData)
		{
			m_IOManager->unregisterFromNewData(outlet, callback, userData);
		}

		void registerToNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler)
		{
			m_IOManager->registerToNewData(outlet, handler);
		}

		void unregisterFromNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler)
		{
			m_IOManager->unregisterFromNewData(outlet, handler);
		}

		void registerToNewData(OutputCallback callback, void *userData)
		{
			m_IOManager->registerToNewData(callback, userData);
		}

		void unregisterFromNewData(OutputCallback callback, void *userData)
		{
			m_IOManager->unregisterFromNewData(callback, userData);
		}

		void registerToNewData(AbstractOutputCallbackHandler &handler)
		{
			m_IOManager->registerToNewData(handler);
		}

		void unregisterFromNewData(AbstractOutputCallbackHandler &handler)
		{
			m_IOManager->unregisterFromNewData(handler);
		}

		EngineData const& getValue(std::string const& paramName) const
		{
			return m_IOManager->getValue(paramName);
		}

		std::string const& getTypename( std::string const& paramName ) const
		{
			return m_IOManager->getTypename( paramName );
		}

		std::string const& getLongTypename( std::string const& paramName ) const
		{
			return m_IOManager->getLongTypename( paramName );
		}

		void setValue(std::string const& paramName, TimestampedData const& value)
		{
			m_IOManager->setValue(paramName, value);
		}

		void linkWith(std::string const& nameIn, AbstractUberBlock &out, std::string const& nameOut)
		{
			m_IOManager->linkWith(nameIn, out, nameOut);
		}

		void setUp()
		{
			m_StateManager->setUp();
		}

		void start()
		{
			m_StateManager->start();
		}

		void setUpdatePolicy( UpdatePolicyImpl const& policy )
		{
			m_StateManager->setUpdatePolicy( policy );
		}

		void stop( const bool blocking, const long timeout )
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

		void prepareForShutDown()
		{
			m_StateManager->prepareForShutDown();
		}

		bool shutDown( const long timeout )
		{
			return m_StateManager->shutDown( timeout );
		}

		Inlet const& getInlet( std::string const& name ) const
		{
			return m_IOManager->getInlet( name );
		}

		Outlet const& getOutlet( std::string const& name ) const
		{
			return m_IOManager->getOutlet( name );
		}

		SetupParameter const& getSetupParameter( std::string const& name ) const
		{
			return m_IOManager->getSetupParameter( name );
		}

		void tryTriggerSubBlock( const unsigned int id, const BlockMessage msg )
		{
			m_StateManager->tryTriggerSubBlock( id, msg );
		}

		void tryTriggerSuperBlock( const unsigned int id, const BlockMessage msg )
		{
			m_StateManager->tryTriggerSuperBlock( id, msg );
		}

	protected:

		AbstractIOManager				*m_IOManager;
		AbstractBlockManager			*m_SubBlockManager;
		AbstractBlockManager			*m_SuperBlockManager;
		AbstractStateManager			*m_StateManager;

	};

}