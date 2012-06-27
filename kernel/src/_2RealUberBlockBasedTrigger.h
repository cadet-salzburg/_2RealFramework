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

	class AbstractUberBlockBasedTrigger : public AbstractUpdateTrigger
	{

	public:

		AbstractUberBlockBasedTrigger() :
			AbstractUpdateTrigger( false ),
			m_Other( nullptr )
		{
		}

		virtual ~AbstractUberBlockBasedTrigger() {}

		void setOther( AbstractUberBlockBasedTrigger &other )
		{
			m_Other = &other;
		}

		void tryTriggerOther( const BlockMessage msg )
		{
			m_Other->tryTriggerUpdate( msg );
		}

		// not really a good solution
		void resetOther()
		{
			m_Other->reset();
		}

	protected:

		virtual void tryTriggerUpdate( const BlockMessage msg ) = 0;

	private:

		AbstractUberBlockBasedTrigger	*m_Other;

	};

	template< class StateMgrDerived >
	class UberBlockBasedTrigger : public AbstractUberBlockBasedTrigger
	{

	public:

		typedef void ( _2REAL_MEMBER_FUNC StateMgrDerived::*Function )( AbstractUberBlockBasedTrigger &trigger );

		UberBlockBasedTrigger( StateMgrDerived &mgr, Function func, const BlockMessage msg ) :
			AbstractUberBlockBasedTrigger(),
			m_Owner( mgr ),
			m_Function( func ),
			m_ExpectedMessage( msg )
		{
			m_Owner.addUberBlockTrigger( *this );
		}

		~UberBlockBasedTrigger()
		{
			m_Owner.removeUberBlockTrigger( *this );
		}

	protected:

		void tryTriggerUpdate( const BlockMessage msg )
		{
			if ( !m_IsOk && msg == m_ExpectedMessage )
			{
				m_IsOk = true;
				(m_Owner.*m_Function)( *this );
			}
		}

	private:

		StateMgrDerived						&m_Owner;
		Function							m_Function;
		BlockMessage						m_ExpectedMessage;

	};

}