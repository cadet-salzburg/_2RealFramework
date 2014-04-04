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

#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	class Exception: public std::exception
	{
	public:

		Exception( std::string const& message );
		Exception( Exception const& src );
		virtual ~Exception() throw();
		Exception& operator=( Exception const& src );

		virtual const char* name() const;
		virtual const char* className() const;
		virtual const char* what() const throw();

		std::string const& message() const;

		virtual Exception* clone() const;
		virtual void rethrow() const;

	protected:

		Exception();
		void setMessage( std::string const& message);

	private:

		std::string		m_Message;

	};

#define DECLARE_EXCEPTION(CLS, BASE)				\
	class CLS: public BASE							\
	{												\
	public:											\
		CLS();										\
		CLS(std::string const& msg);				\
		CLS(CLS const& src);						\
		~CLS() throw();										\
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
	CLS::~CLS() throw()										\
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

	DECLARE_EXCEPTION( CreationFailure, Exception )
	DECLARE_EXCEPTION( BundleImportFailure, Exception )
	//DECLARE_EXCEPTION( InvalidStateChangeException, Exception )
	//DECLARE_EXCEPTION( InvalidTypeException, Exception )
	DECLARE_EXCEPTION( NotFound, Exception )
	DECLARE_EXCEPTION( AlreadyExists, Exception )
	DECLARE_EXCEPTION( TypeMismatch, Exception )
	DECLARE_EXCEPTION( HandleAccessFailure, Exception )
	//DECLARE_EXCEPTION( TimeOutException, Exception )
	//DECLARE_EXCEPTION( InvalidNameException, Exception )
	//DECLARE_EXCEPTION( XMLFormatException, Exception )
	//DECLARE_EXCEPTION( OutOfRangeException, Exception )
	//DECLARE_EXCEPTION( IllegalActionException, Exception )

}
