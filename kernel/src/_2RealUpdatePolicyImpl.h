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

#include "_2RealTriggerTypes.h"

#include <string>
#include <memory>

namespace _2Real
{

	class AbstractInletDefault
	{

	public:

		virtual AbstractInletBasedTrigger * createTrigger( std::string const& name ) = 0;

	};

	template< typename Comparison >
	class InletDefault : public AbstractInletDefault
	{

	public:

		AbstractInletBasedTrigger * createTrigger( std::string const& name )
		{
			return new InletBasedTrigger< Comparison >( name );
		}

	};

	class AbstractBlockDefault
	{

	public:

		virtual AbstractBlockBasedTrigger * createTrigger( std::string const& name ) = 0;

	};

	template< BlockMessage TriggerState >
	class BlockDefault : public AbstractBlockDefault
	{

	public:

		AbstractBlockBasedTrigger * createTrigger( std::string const& name )
		{
			return new BlockBasedTrigger< TriggerState >( name );
		}

	};

	class UpdatePolicyImpl
	{

	public:

		typedef std::shared_ptr< AbstractInletDefault > InletDefaultPtr;
		typedef std::shared_ptr< AbstractBlockDefault > BlockDefaultPtr;

		UpdatePolicyImpl();
		~UpdatePolicyImpl();

		void clear();
		void setInletDefault( UpdatePolicyImpl::InletDefaultPtr &inletDefault );
		void setBlockDefault( UpdatePolicyImpl::BlockDefaultPtr &blockDefault );
		void addInletBasedTrigger( std::string const& inletName, UpdatePolicyImpl::InletDefaultPtr &trigger );
		void addTimeBasedTrigger( const long time );
		void addBlockBasedTrigger( std::string const& blockName, UpdatePolicyImpl::BlockDefaultPtr &trigger );

		AbstractTimeBasedTrigger * getTimeBasedTrigger() const;
		AbstractInletBasedTrigger * getTriggerForInlet( std::string const& inletName ) const;
		AbstractBlockBasedTrigger * getBlockBasedTrigger( std::string const& blockName ) const;

		bool hasTimeBasedTrigger() const;
		bool hasTriggerForInlet( std::string const& inletName ) const;

	private:

		typedef std::shared_ptr< AbstractInletDefault > InletDefaultPtr;
		typedef std::shared_ptr< AbstractBlockDefault > BlockDefaultPtr;

		long												m_Time;
		std::map< std::string, InletDefaultPtr >			m_Inlets;
		std::map< std::string, BlockDefaultPtr >			m_Blocks;
		InletDefaultPtr										m_InletDefault;
		BlockDefaultPtr										m_BlockDefault;

	};

}