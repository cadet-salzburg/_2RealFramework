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
#include "_2RealIdentifier.h"

namespace _2Real
{

	class Exception : public std::runtime_error
	{

	public:

		Exception(std::string const& message) :
			std::runtime_error(message)
		{
		}

		virtual ~Exception()
		{
		}

	};

	/**
	*	thrown by service or plugin context on parameter query
	*/

	class InvalidParameterException : public Exception
	{

	public:

		InvalidParameterException(std::string const& type, std::string const& name) :
			Exception(std::string("invalid parameter: ").append(type).append(" - ").append(name).append(" does not exist"))
		{
		}

	};

	/**
	*	thrown by service context on register service
	*/

	class InvalidServiceException : public Exception
	{

	public:

		InvalidServiceException(std::string const& name) :
			Exception(std::string("invalid service: ").append(name).append(" could not be registered"))
		{
		}

	};

	/**
	*	thrown by any shared any on extraction
	*/

	class DatatypeMismatchException : public Exception
	{

	public:

		DatatypeMismatchException(std::string const& t1, std::string const& t2, std::string const& name) :
			Exception(std::string("datatype mismatch of parameter ").append(name).append(": ").append(t1).append(" vs ").append(t2))
		{
		}

	};

	/**
	*	thrown by system if runnable fails on run
	*/

	class RunnableException : public Exception
	{

	public:

		RunnableException(std::string const& message, Identifier const& sender, Identifier const& system) :
			Exception(message),
			m_System(system),
			m_Sender(sender)
		{
		}

		Identifier const& system()
		{
			return m_System;
		}

		Identifier const& sender()
		{
			return m_Sender;
		}

	private:

		Identifier	m_System;
		Identifier	m_Sender;

	};

	/**
	*	thrown by metadata reader
	*/

	class MetadataFormatException : public Exception
	{

	public:

		MetadataFormatException(std::string const& message) :
			Exception(message)
		{
		}

	};


}