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
#include "_2RealAbstractUpdateTrigger.h"
#include "_2RealAbstractStateManager.h"

#include <map>

namespace _2Real
{

#if defined( _WIN32 )
	#define _2REAL_MEMBER_FUNC __thiscall
#elif defined( _WIN64 )
	#define _2REAL_MEMBER_FUNC __thiscall
#else
	#define _2REAL_MEMBER_FUNC
#endif

	enum BlockMessage
	{
		BLOCK_OK						=	0x00,
		BLOCK_READY						=	0x01,
		BLOCK_FINISHED					=	0x02,
		BLOCK_NOT_OK					=	0x04,
	};

	class UberBlockBasedTrigger : public AbstractUpdateTrigger
	{

	public:

		UberBlockBasedTrigger( AbstractStateManager &mine, const BlockMessage msg ) :
			AbstractUpdateTrigger( false ),
			m_Mine( mine ),
			m_ExpectedMessage( msg ),
			m_Other( nullptr )
		{
		}

		~UberBlockBasedTrigger()
		{
		}

		void setOther( UberBlockBasedTrigger &other )
		{
			m_Other = &other;
		}

		void tryTriggerOther( const BlockMessage msg )
		{
			m_Other->tryTriggerUpdate( msg );
		}

	protected:

		void tryTriggerUpdate( const BlockMessage msg )
		{
			if ( !m_IsOk && msg == m_ExpectedMessage )
			{
				m_IsOk = true;
				m_Mine.tryTriggerUberBlock( *this );
			}
			//else if ( m_IsOk )
			//{
			//}
			//else
			//{
			//}
		}

	private:

		AbstractStateManager		&m_Mine;
		UberBlockBasedTrigger		*m_Other;
		BlockMessage				m_ExpectedMessage;

	};


	// this needs to happen at the right moment, i.e. after
	// an update cycle is complete
	class TriggerLink
	{

	public:

		TriggerLink( AbstractUberBlock &b1, const BlockMessage m1, AbstractUberBlock &b2, const BlockMessage m2 )
		{
			//m_S1 = &b1.getStateManager();
			//m_S2 = &b2.getStateManager();
			//UberBlockBasedTrigger *t1 = new UberBlockBasedTrigger( *m_S1, m1 );
			//UberBlockBasedTrigger *t2 = new UberBlockBasedTrigger( *m_S2, m2 );
			//t1->setOther( *t2 );
			//t2->setOther( *t1 );
			//m_S1->addUberBlockTrigger( *t1 );
			//m_S2->addUberBlockTrigger( *t2 );
			
			//m_T1 = b1.createUberBlockTrigger( m1 );
			//m_T2 = b2.createUberBlockTrigger( m2 );
			//m_T1->setOther( *m_T2 );
			//m_T2->setOther( *m_T1 );
		}

		~TriggerLink()
		{
			//m_S1->removeUberBlockTrigger( *m_T1 );
			//m_S2->removeUberBlockTrigger( *m_T2 );

			/*delete m_T1;
			delete m_T2;*/
		}

	private:

		//AbstractStateManager		*m_S1;
		//AbstractStateManager		*m_S2;
		UberBlockBasedTrigger		*m_T1;
		UberBlockBasedTrigger		*m_T2;

	};

}