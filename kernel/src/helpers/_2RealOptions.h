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

#include "helpers/_2RealAny.h"

#include <set>

namespace _2Real
{

	template< typename TData >
	class Option
	{

	public:

		Option( TData const& v, std::string const& d ) : m_Val( v ), m_Desc( d ) {}

		bool operator<( Option< TData > const& other ) const
		{
			return m_Val < other.m_Val;
		}

		TData			m_Val;
		std::string		m_Desc;

	};

	class AnyOption
	{

	public:

		template< typename TData >
		AnyOption( TData const& v, std::string const& d ) : m_Val( v ), m_Desc( d ) {}

		bool isEqualTo( Any const& any ) const
		{
			return m_Val.isEqualTo( any );
		}

		bool operator<( AnyOption const& other ) const
		{
			return m_Val.isLessThan( other.m_Val );
		}

		std::string const& getDescription() const
		{
			return m_Desc;
		}

		Any const& getValue() const
		{
			return m_Val;
		}

	private:

		Any				m_Val;
		std::string		m_Desc;

	};

	template< typename TData >
	class Options
	{

	public:

		Options( TData const& v, std::string const& d )
		{
			m_Set.insert( AnyOption( v, d ) );
		}

		Options& operator()( TData const& v, std::string const& d )
		{
			m_Set.insert( AnyOption( v, d ) );
			return *this;
		}

		operator std::set< AnyOption > const& () const
		{
			return m_Set;
		}

	private:

		std::set< AnyOption >	m_Set;

	};

	class AnyOptionSet
	{

	private:

		typedef std::set< AnyOption >						Options;
		typedef std::set< AnyOption >::iterator				OptionIterator;
		typedef std::set< AnyOption >::const_iterator		OptionConstIterator;

		Options			m_Options;

	public:

		AnyOptionSet() : m_Options() {}

		AnyOptionSet( std::set< AnyOption > const& options ) : m_Options( options ) {}

		bool isEmpty() const
		{
			return m_Options.empty();
		}

		bool isOption( Any const& any ) const
		{
			for ( OptionConstIterator it = m_Options.begin(); it != m_Options.end(); ++it )
			{
				if ( it->isEqualTo( any ) )
				{
					return true;
				}
			}
			return false;
		}

		template< typename TData >
		std::set< Option< TData > > extract()
		{
			std::set< Option< TData > > result;
			for ( AnyOptionSet::OptionIterator it = options.m_Options.begin(); it != options.m_Options.end(); ++it )
			{
				TData const& v = it->getValue().extract< TData >();
				result.insert( Option< TData >( v, it->getDescription() ) );
			}
			return result;
		}
	};
}