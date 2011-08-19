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

#include "_2RealException.h"

namespace _2Real
{

	Exception Exception::s_Success = Exception(25);
	Exception Exception::s_Failure = Exception(100);
	Exception Exception::s_NoCopy = Exception(150);

	Exception const& Exception::success()
	{
		return s_Success;
	}

	Exception const& Exception::failure()
	{
		return s_Failure;
	}

	Exception const& Exception::noCopy()
	{
		return s_NoCopy;
	}

	Exception::Exception() : m_Code(0)
	{
	}

	Exception::Exception(unsigned int const& _nr) : m_Code(_nr)
	{
	}

	bool Exception::operator==(Exception const& _rhs)
	{
		return (m_Code == _rhs.m_Code);
	}

	bool Exception::operator!=(Exception const& _rhs)
	{
		return (m_Code != _rhs.m_Code);
	}

}