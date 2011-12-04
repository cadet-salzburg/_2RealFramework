#pragma once

#include "_2RealException.h"

#include <typeinfo>
#include <iostream>

#include "Poco/SharedPtr.h"

namespace _2Real
{

	class EngineData
	{

	public:
	
		EngineData() :
			m_Content()
		{
		}

		template< typename T >
		EngineData(T* value) :
			m_Content(new TypeHolder< T >(value))
		{
		}

		template< typename T >
		EngineData(T value) :
			m_Content(new TypeHolder< T >(new T(value)))
		{
		}

		EngineData(EngineData const& src) :
			m_Content(src.m_Content)
		{
		}

		~EngineData()
		{
			m_Content.assign(NULL);
		}

		bool empty() const
		{
			return m_Content.isNull();
		}

		std::type_info const& type() const
		{
			if (m_Content.isNull())
			{
				return typeid(void);
			}
			else
			{
				return m_Content->type();
			}
		}

		void clone(EngineData const& src)
		{
			m_Content.assign(src.m_Content->clone());
		}

		void create(EngineData const& src)
		{
			m_Content.assign(src.m_Content->create());
		}

		private:
	
			class DataHolder
			{
	
			public:
	
				virtual ~DataHolder() {}
				virtual const std::type_info& type() const = 0;
				virtual DataHolder* clone() const = 0;
				virtual DataHolder* create() const = 0;

			};

			template< typename T >
			class TypeHolder : public DataHolder
			{
	
			public:

				TypeHolder() :
					m_Shared(NULL)
				{
				}

				TypeHolder(T *value) :
					m_Shared(value)
				{
				}

				TypeHolder(TypeHolder< T > const& src) :
					m_Shared(src.m_Shared)
				{
				}

				TypeHolder& operator=(TypeHolder< T > const& src)
				{
					if (this == &src)
					{
						return *this;
					}

					m_Shared = src.m_Shared;

					return *this;
				}

				virtual ~TypeHolder()
				{
					m_Shared.assign(NULL);
				}

				virtual std::type_info const& type() const
				{
					return typeid(T);
				}

				virtual DataHolder* create() const
				{
					T *newContent = new T();
					return new TypeHolder(newContent);
				}

				virtual DataHolder* clone() const
				{
					T *newContent = new T(*m_Shared.get());
					return new TypeHolder(newContent);
				}

				Poco::SharedPtr< T >	m_Shared;
			};

		template< typename T >
		friend Poco::SharedPtr< T > Extract(EngineData any);

		Poco::SharedPtr< DataHolder >				m_Content;

	};

	template< typename T >
	Poco::SharedPtr< T > Extract(EngineData any)
	{
		if (any.type() == typeid(T))
		{
			EngineData::DataHolder *ptr = any.m_Content.get();
			return static_cast< EngineData::TypeHolder< T > * >(ptr)->m_Shared;
		}
		else
		{
			throw BadCastException(any.type().name(), typeid(T).name());
		}
	}

}