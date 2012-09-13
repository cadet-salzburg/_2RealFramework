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

#include "bundle/_2RealInletHandle.h"
#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"
#include "engine/_2RealTimestampedData.h"

namespace _2Real
{
	class BasicInlet;

	class AbstractInlet : private NonCopyable< AbstractInlet >, private Identifiable< AbstractInlet >, private Handleable< AbstractInlet, bundle::InletHandle >
	{

	public:

		AbstractInlet( AbstractUberBlock &owningBlock, std::string const& name );
		virtual ~AbstractInlet() {}

		using Identifiable< AbstractInlet >::getFullName;
		using Identifiable< AbstractInlet >::getName;

		using Handleable< AbstractInlet, bundle::InletHandle >::getHandle;
		using Handleable< AbstractInlet, bundle::InletHandle >::registerHandle;
		using Handleable< AbstractInlet, bundle::InletHandle >::unregisterHandle;

		virtual BasicInlet &		operator[]( const unsigned int index ) = 0;
		virtual bool				isMultiInlet() const = 0;
		virtual unsigned int		getSize() const = 0;

	};

	class BasicInlet : public AbstractInlet
	{

	public:

		BasicInlet( AbstractUberBlock &owningBlock, std::string const& name );

		BasicInlet &				operator[]( const unsigned int index ) { return *this; }
		bool						isMultiInlet() const { return false; }
		unsigned int				getSize() const { return 1; }

		TimestampedData const&		getCurrentData() const;
		void						setData( TimestampedData const& data );
		bool						hasUpdated() const;
		bool						hasChanged() const;

	private:

		TimestampedData				m_LastData;
		TimestampedData				m_CurrentData;

	};

	class MultiInlet : public AbstractInlet
	{

	public:

		typedef std::vector< BasicInlet * >						BasicInlets;
		typedef std::vector< BasicInlet * >::iterator			BasicInletIterator;
		typedef std::vector< BasicInlet * >::const_iterator		BasicInletsConstInterator;

		MultiInlet( AbstractUberBlock &owningBlock, std::string const& name );
		~MultiInlet();

		BasicInlet &				operator[]( const unsigned int index );
		bool						isMultiInlet() const { return true; }
		unsigned int				getSize() const { return m_Inlets.size(); }

		void						addBasicInlet( BasicInlet &inlet );
		void						removeBasicInlet( BasicInlet &inlet );

	private:

		// sync = done in inlet io class
		BasicInlets					m_Inlets;

	};
}