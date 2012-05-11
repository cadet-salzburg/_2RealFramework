#pragma once

#include "_2RealException.h"
#include "_2RealHelpers.h"
#include "_2RealTypeHolder.h"

#include <vector>
#include <typeinfo>
#include <sstream>

#include "Poco/SharedPtr.h"

namespace _2Real
{

	class EngineData
	{

	public:
	
		friend std::ostream& operator<<(std::ostream& out, EngineData const& data)
		{
			data.writeTo(out);
			return out;
		}

		friend std::istream& operator>>(std::istream& in, EngineData &data)
		{
			data.readFrom(in);
			return in;
		}

		EngineData();
		EngineData(EngineData const& src);
		EngineData& operator=(EngineData const& src);
		~EngineData();

		template< typename DataType >
		EngineData(DataType *const value);

		template< typename DataType >
		EngineData(DataType const& value);

		EngineData(AbstractDataHolder *holder);

		bool isEmpty() const;
		std::type_info const& typeinfo() const;
		void clone(EngineData const& src);
		void create(EngineData const& src);
		//const std::string toString() const;

	private:

		template< typename DataType >
		friend Poco::SharedPtr< DataType > const& Extract(EngineData const& data);

		Poco::SharedPtr< AbstractDataHolder >	m_Content;

		void writeTo(std::ostream &out) const;
		void readFrom(std::istream &in);

	};

	template< typename DataType >
	Poco::SharedPtr< DataType > const& Extract(EngineData const& data)
	{
		if (data.typeinfo() == typeid(DataType))
		{
			AbstractDataHolder const* ptr = data.m_Content.get();
			return static_cast< DataHolder< DataType > const* >(ptr)->m_Data;
		}
		else
		{
			std::ostringstream msg;
			msg << "type of data " << data.typeinfo().name() << " does not match template parameter " << typeid(DataType).name() << std::endl;
			throw TypeMismatchException(msg.str());
		}
	}

	inline EngineData::EngineData(AbstractDataHolder *holder)
	{
		m_Content.assign(holder);
	}

	template< typename DataType >
	EngineData::EngineData(DataType *const value)
	{
		DataHolder< DataType > *holder = new DataHolder< DataType >(value);
		m_Content.assign(holder);
	}

	template< typename DataType >
	EngineData::EngineData(DataType const& value)
	{
		DataType *newData = new DataType(value);
		DataHolder< DataType > *holder = new DataHolder< DataType >(newData);
		m_Content.assign(holder);
	}

	inline EngineData::EngineData() :
		m_Content()
	{
	}

	inline EngineData::EngineData(EngineData const& src) :
		m_Content(src.m_Content)
	{
	}

	inline EngineData& EngineData::operator=(EngineData const& src)
	{
		if (this == &src)
		{
			return *this;
		}

		m_Content = src.m_Content;

		return *this;
	}

	inline EngineData::~EngineData()
	{
		m_Content.assign(NULL);
	}

	inline bool EngineData::isEmpty() const
	{
		return m_Content.isNull();
	}

	inline void EngineData::writeTo(std::ostream &out) const
	{
		if (!m_Content.isNull())
		{
			m_Content->writeTo(out);
		}
		else
		{
		}
	}

	inline void EngineData::readFrom(std::istream &in)
	{
		if (!m_Content.isNull())
		{
			m_Content->readFrom(in);
		}
		else
		{
		}
	}

	//inline const std::string EngineData::toString() const
	//{
	//	if (!m_Content.isNull())
	//	{
	//		return m_Content->toString();
	//	}
	//	else
	//	{
	//		return std::string("void");
	//	}
	//}

	inline std::type_info const& EngineData::typeinfo() const
	{
		if (m_Content.isNull())
		{
			return typeid(void);
		}
		else
		{
			return m_Content->typeinfo();
		}
	}

	inline void EngineData::clone(EngineData const& src)
	{
		m_Content.assign(src.m_Content->clone());
	}

	inline void EngineData::create(EngineData const& src)
	{
		m_Content.assign(src.m_Content->create());
	}

}