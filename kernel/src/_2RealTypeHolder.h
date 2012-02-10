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

#include <iostream>

#include "Poco/SharedPtr.h"

namespace _2Real
{
	class AbstractDataHolder
	{
	
	public:
	
		virtual ~AbstractDataHolder();
		virtual std::type_info const& typeinfo() const = 0;
		virtual AbstractDataHolder* clone() const = 0;
		virtual AbstractDataHolder* create() const = 0;
		//virtual const std::string toString() const = 0;
		virtual void writeTo(std::ostream &out) const = 0;
		virtual void readFrom(std::istream &in) = 0;

	};

	inline AbstractDataHolder::~AbstractDataHolder()
	{
	}

	template< typename DataType >
	class DataHolder : public AbstractDataHolder
	{
	
	public:

		DataHolder(DataType *value);
		DataHolder(DataType const& value);
		DataHolder(DataHolder< DataType > const& src);
		DataHolder& operator=(DataHolder< DataType > const& src);
		~DataHolder();

		std::type_info const& typeinfo() const;
		AbstractDataHolder* create() const;
		AbstractDataHolder* clone() const;
		//const std::string toString() const;

		Poco::SharedPtr< DataType >		m_Data;

		void writeTo(std::ostream &out) const;
		void readFrom(std::istream &in);

	};

	template< typename DataType >
	DataHolder< DataType >::DataHolder(DataType const& value)
	{
		m_Data.assign(new DataType(value));
	}

	template< typename DataType >
	DataHolder< DataType >::DataHolder(DataType *value)
	{
		m_Data.assign(value);
	}

	template< typename DataType >
	DataHolder< DataType >::DataHolder(DataHolder< DataType > const& src) :
		m_Data(src.m_Data)
	{
	}

	template< typename DataType >
	DataHolder< DataType >& DataHolder< DataType >::operator=(DataHolder< DataType > const& src)
	{
		if (this == &src)
		{
			return *this;
		}

		m_Data = src.m_Data;

		return *this;
	}

	template< typename DataType >
	DataHolder< DataType >::~DataHolder()
	{
		m_Data.assign(NULL);
	}

	template< typename DataType >
	std::type_info const& DataHolder< DataType >::typeinfo() const
	{
		return typeid(DataType);
	}

	template< typename DataType >
	void DataHolder< DataType >::writeTo(std::ostream &out) const
	{
		out << *m_Data.get();
	}

	template< typename DataType >
	void DataHolder< DataType >::readFrom(std::istream &in)
	{
		in >> *m_Data.get();
	}

	//template< typename DataType >
	//const std::string DataHolder< DataType >::toString() const
	//{
	//	std::ostringstream msg;
	//	msg << *m_Data.get();
	//	return msg.str();
	//}

	template< typename DataType >
	AbstractDataHolder * DataHolder< DataType >::create() const
	{ 
		DataType *newContent = new DataType();
		return new DataHolder(newContent);
	}

	template< typename DataType >
	AbstractDataHolder * DataHolder< DataType >::clone() const
	{
		DataType *newContent = new DataType(*m_Data.get());
		return new DataHolder(newContent);
	}

}