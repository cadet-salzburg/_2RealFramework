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

#include <stdexcept>
#include <typeinfo.h>

namespace _2Real
{

	class Exception: public std::exception
	{
	public:

		Exception(std::string const& message);
		Exception(Exception const& src);
		~Exception();
		Exception& operator=(Exception const& src);

		virtual const char* name() const;
		virtual const char* className() const;
		virtual const char* what() const;

		std::string const& message() const;

		virtual Exception* clone() const;
		virtual void rethrow() const;

	protected:

		Exception();
		void setMessage(const std::string& message);

	private:

		std::string		m_Message;

	};

	inline std::string const& Exception::message() const
	{
		return m_Message;
	}


	inline void Exception::setMessage(std::string const& message)
	{
		m_Message = message;
	}

#define DECLARE_EXCEPTION(CLS, BASE)				\
	class CLS: public BASE							\
	{												\
	public:											\
		CLS();										\
		CLS(std::string const& msg);				\
		CLS(CLS const& src);						\
		~CLS();										\
		CLS& operator=(CLS const& src);				\
		const char* name() const;					\
		const char* className() const;				\
		Exception* clone() const;					\
		void rethrow() const;						\
	};

#define IMPLEMENT_EXCEPTION(CLS, BASE, NAME)		\
	CLS::CLS(): BASE()								\
	{												\
	}												\
	CLS::CLS(std::string const& msg): BASE(msg)		\
	{												\
	}												\
	CLS::CLS(CLS const& src): BASE(src)				\
	{												\
	}												\
	CLS::~CLS()										\
	{												\
	}												\
	CLS& CLS::operator=(CLS const& src)				\
	{												\
		BASE::operator=(src);						\
		return *this;								\
	}												\
	const char* CLS::name() const					\
	{												\
		return NAME;								\
	}												\
	const char* CLS::className() const				\
	{												\
		return typeid(*this).name();				\
	}												\
	Exception* CLS::clone() const					\
	{												\
		return new CLS(*this);						\
	}												\
	void CLS::rethrow() const						\
	{												\
		throw *this;								\
	}

	DECLARE_EXCEPTION(XMLFormatException, Exception)
	DECLARE_EXCEPTION(InvalidTypeException, Exception)
	DECLARE_EXCEPTION(NotFoundException, Exception)
	DECLARE_EXCEPTION(AlreadyExistsException, Exception)
	DECLARE_EXCEPTION(InvalidIdentifierException, Exception)
	DECLARE_EXCEPTION(TypeMismatchException, Exception)
	DECLARE_EXCEPTION(UninitializedHandleException, Exception)
	DECLARE_EXCEPTION(PluginException, Exception)
	DECLARE_EXCEPTION(ServiceException, Exception)

}