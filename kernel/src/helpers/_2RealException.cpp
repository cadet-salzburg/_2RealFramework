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

#include "helpers/_2RealException.h"

using std::string;

namespace _2Real
{

	Exception::Exception()
	{
	}

	Exception::Exception( string const& message ) :
		m_Message( message )
	{
	}

	Exception::Exception( Exception const& src ):
		std::exception( src ),
		m_Message( src.m_Message )
	{
	}

	Exception::~Exception() throw()
	{
	}

	Exception& Exception::operator=( Exception const& src )
	{
		m_Message = src.m_Message;
		return *this;
	}

	const char* Exception::name() const
	{
		return "Exception";
	}

	const char* Exception::className() const
	{
		return typeid( *this ).name();
	}

	const char* Exception::what() const throw()
	{
		return name();
	}

	Exception* Exception::clone() const
	{
		return new Exception( *this );
	}

	void Exception::rethrow() const
	{
		throw *this;
	}

	string const& Exception::message() const
	{
		return m_Message;
	}

	void Exception::setMessage( string const& message )
	{
		m_Message = message;
	}

	IMPLEMENT_EXCEPTION( BundleImportFailure, Exception, "a dll ( bundle ) failed to load properly ( BundleImportFailure Exception )" )
	//IMPLEMENT_EXCEPTION( InvalidStateChangeException, Exception, "invalid state change exception" )
	//IMPLEMENT_EXCEPTION( InvalidTypeException, Exception, "invalid type exception" )
	IMPLEMENT_EXCEPTION( NotFound, Exception, "an object was not found ( NotFound Exception )" )
	IMPLEMENT_EXCEPTION( AlreadyExists, Exception, "a named object already exists ( AlreadyExists Exception )" )
	IMPLEMENT_EXCEPTION( TypeMismatch, Exception, "a type mismatch occured ( TypeMismatch Exception )" )
	IMPLEMENT_EXCEPTION( HandleAccessFailure, Exception, "an uninitialized handle was accessed ( HandleAccessFailure Exception )" )
	IMPLEMENT_EXCEPTION( CreationFailure, Exception, "an object could not be created ( CreationFailure Exception )" )
	//IMPLEMENT_EXCEPTION( TimeOutException, Exception, "time out exception" )
	//IMPLEMENT_EXCEPTION( InvalidNameException, Exception, "invalid name exception" )
	//IMPLEMENT_EXCEPTION( XMLFormatException, Exception, "xml format exception" )
	//IMPLEMENT_EXCEPTION( OutOfRangeException, Exception, "out of range exception" )
	//IMPLEMENT_EXCEPTION( IllegalActionException, Exception, "illegal action exception" )

}
