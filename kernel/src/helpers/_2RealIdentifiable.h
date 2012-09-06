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

#include <list>
#include <string>
#include <sstream>

namespace _2Real
{

	class AbstractInlet;
	class Outlet;
	class Bundle;
	class AbstractUberBlock;

	class AbstractIdentifiable
	{

	protected:

		virtual ~AbstractIdentifiable();

		typedef std::list< AbstractIdentifiable const* >					Ids;
		typedef std::list< AbstractIdentifiable const* >::iterator			IdIterator;
		typedef std::list< AbstractIdentifiable const* >::const_iterator	IdConstIterator;

		AbstractIdentifiable( Ids const& ids, std::string const& type, std::string const& name );
		const Ids getIds() const;

		const std::string getFullName() const;
		std::string const& getName() const;
		std::string const& getType() const;
		void setName( std::string const& name );

	private:

		std::string				const m_Type;
		std::string				m_Name;
		Ids						m_Ids;

	};

	template< typename T >
	class Identifiable : public AbstractIdentifiable
	{
	protected:
		Identifiable( Ids const& ids, std::string const& type, std::string const& name ) : AbstractIdentifiable( ids, type, name ) {}
	};

	template< >
	class Identifiable< Bundle > : public AbstractIdentifiable
	{
	protected:
		Identifiable( Ids const& ids, std::string const& name ) : AbstractIdentifiable( ids, "bundle", name ) {}
	};

	template< >
	class Identifiable< AbstractUberBlock > : public AbstractIdentifiable
	{
	protected:
		Identifiable( Ids const& ids, std::string const& name ) : AbstractIdentifiable( ids, "block", name ) {}
	};

	template< >
	class Identifiable< Outlet > : public AbstractIdentifiable
	{
	protected:
		Identifiable( Ids const& ids, std::string const& name ) : AbstractIdentifiable( ids, "outlet", name ) {}
	};

	template< >
	class Identifiable< AbstractInlet > : public AbstractIdentifiable
	{
	protected:
		Identifiable( Ids const& ids, std::string const& name ) : AbstractIdentifiable( ids, "inlet", name ) {}
	};

}