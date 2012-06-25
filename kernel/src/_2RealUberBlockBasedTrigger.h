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

#include "_2RealUpdateTrigger.h"
#include "_2RealAbstractUberBlock.h"

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

	class AbstractUberBlockBasedTrigger : public UpdateTrigger
	{

	public:

		AbstractUberBlockBasedTrigger( AbstractUberBlock &owner, AbstractUberBlock &other ) :
			m_Condition( false ),
			m_Owner( owner ),
			m_Other( other )
		{
		}

		virtual ~AbstractUberBlockBasedTrigger() {}

		void reset() { m_Condition = false; }
		bool isOk() const { return m_Condition; }

		virtual void tryTriggerOther( const BlockMessage msg ) = 0;
		virtual bool tryTriggerUpdate( const BlockMessage msg ) = 0;

	protected:

		bool					m_Condition;

	private:

		AbstractUberBlock		&m_Owner;
		AbstractUberBlock		&m_Other;

	};

	typedef std::map< unsigned int, AbstractUberBlockBasedTrigger * >	UberBlockBasedTriggerMap;

	template< class B >
	class UberBlockBasedTrigger : public AbstractUberBlockBasedTrigger
	{

	public:

		typedef void ( _2REAL_MEMBER_FUNC B::*Function )( const unsigned int, const BlockMessage msg );

		UberBlockBasedTrigger( AbstractUberBlock &owner, B &other, Function func, const BlockMessage msg ) :
			AbstractUberBlockBasedTrigger( owner, other ),
			m_Id( owner.getId() ),
			m_Other( other ),
			m_Function( func ),
			m_Message( msg )
		{
		}

		void tryTriggerOther( const BlockMessage msg )
		{
			( m_Other.*m_Function )( m_Id, msg );
		}

		bool tryTriggerUpdate( const BlockMessage msg )
		{
			if ( !m_Condition && msg == m_Message )
			{
				m_Condition = true;
			}
			return m_Condition;
		}

	private:

		unsigned int		const m_Id;
		B					&m_Other;
		Function			m_Function;
		BlockMessage		const m_Message;

	};

}