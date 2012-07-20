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

namespace _2Real
{

	//template< typename TData >
	//class Option
	//{

	//public:

	//	Option( TData const& v, std::string const& d ) : m_Val( v ), m_Desc( d ) {}

	//private:

	//	TData			m_Val;
	//	std::string		m_Desc;

	//};

	//template< typename TData >
	//class Options
	//{

	//public:

	//	Options( TData const& v, std::string const& d )
	//	{
	//		m_List.push_back( Option< TData >( v, d ) );
	//	}

	//	Options& operator()( TData const& v, std::string const& d )
	//	{
	//		m_List.push_back( Option< TData >( v, d ) );
	//		return *this;
	//	}

	//	operator std::list< Option< TData > > const& () const
	//	{
	//		return m_List;
	//	}

	//private:

	//	std::list< Option< TData > >	m_List;

	//};

	//class AbstractOptionsHolder
	//{

	//public:

	//	virtual const std::string getOptionType() const = 0;
	//	virtual bool isValidOption( Any const& any ) const = 0;

	//};

	//template< typename TData >
	//class OptionsHolder : public AbstractOptionsHolder
	//{

	//public:

	//	OptionsHolder( Options< TData > const& options ) :
	//		m_Options( options )
	//	{
	//	}

	//	const std::string getOptionType() const
	//	{
	//		return std::string();
	//	}

	//	bool isValidOption( Any const& any ) const
	//	{
	//	}

	//private:

	//	std::list< Option< TData > >		m_Options;

	//};

	//class AnyOptions
	//{

	//public:

	//	AnyOptions() :
	//		m_Holder( nullptr )
	//	{
	//	}

	//	~AnyOptions()
	//	{
	//		delete m_Holder;
	//	}

	//	template< typename TData >
	//	AnyOptions( Options< TData > const& o ) :
	//		m_Holder( new OptionsHolder< TData >( o ) )
	//	{
	//	}

	//	const std::string getOptionType() const
	//	{
	//		return m_Holder->getOptionType();
	//	}

	//	bool isValidOption( Any const& any )
	//	{
	//		return m_Holder->isValidOption( any );
	//	}

	//private:

	//	AbstractOptionsHolder		*m_Holder;

	//};

}