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

#include "_2RealErrorState.h"

namespace _2Real
{

	ErrorState ErrorState::s_Success = ErrorState(25);
	ErrorState ErrorState::s_Failure = ErrorState(100);

	ErrorState const& ErrorState::success()
	{
		return s_Success;
	}

	ErrorState const& ErrorState::failure()
	{
		return s_Failure;
	}

	ErrorState::ErrorState() : m_Code(0)
	{
	}

	ErrorState::ErrorState(unsigned int const& _nr) : m_Code(_nr)
	{
	}

	bool ErrorState::operator==(ErrorState const& _rhs)
	{
		return (m_Code == _rhs.m_Code);
	}

	bool ErrorState::operator!=(ErrorState const& _rhs)
	{
		return (m_Code != _rhs.m_Code);
	}

}