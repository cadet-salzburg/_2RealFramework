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

	//exception in xml file
	//e.g.: unknown key, plugin name != classname etc
	DECLARE_EXCEPTION(XMLFormatException, Exception)

	//occurs only during the plugin's getMetatdata function
	//if any type of parameter (setup, input, output) is declared with
	//a type not allowed in the framework
	DECLARE_EXCEPTION(InvalidTypeException, Exception)

	//occurs anytime sth is not found:
	//in a plugin/service, when trying to access a non-existant parameter
	//in the application, when trying to create a non-existant service or set a non-existant parameter
	//etc
	DECLARE_EXCEPTION(NotFoundException, Exception)

	//occurs in the plugin anytime something is doubly defined, e.g. a plugin registering a service twice
	//or metadata declaring a parameter twice
	DECLARE_EXCEPTION(AlreadyExistsException, Exception)

	//occurs only in calls from the application, if the identifier does not belong to the right kind
	//of entity, e.g. calling linkSlots() on a plugin
	//this might actually not be necessary and could be replaced by a boolean return value indicating success / failure
	DECLARE_EXCEPTION(InvalidIdentifierException, Exception)

	//occurs when there is a mismatch between the declared type of any parameter (according to the metadata)
	//and the template parameter used (note that all functions involving parameters are function templates)
	DECLARE_EXCEPTION(TypeMismatchException, Exception)

	//occurs only in the service, when accessing a handle that is not initialized to an actual i/o slot
	DECLARE_EXCEPTION(UninitializedHandleException, Exception)

	//something went so very wrong while calling any of an IService's functions
	//that the service in question is only good for putting it out of its misery. YAY!
	DECLARE_EXCEPTION(ServiceException, Exception)

	//something went so very wrong while calling any of an IPluginActivator's functions
	//that the plugin in question is only good for putting it out of its misery. MOAR YAY!
	DECLARE_EXCEPTION(PluginException, Exception)

	DECLARE_EXCEPTION(SetupException, Exception);

	DECLARE_EXCEPTION(StartException, Exception);

	DECLARE_EXCEPTION(InvalidNameException, Exception);

}