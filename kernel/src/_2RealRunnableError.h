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

#include "_2RealIdentifier.h"
#include "_2RealException.h"

namespace _2Real
{

	class BlockError
	{

	public:

		BlockError(Exception const& exception, BlockIdentifier const& runnableId);

		BlockIdentifier const& getId() const;
		Exception const& getException() const;

	private:

		Exception	m_Exception;
		BlockIdentifier	m_Runnable;

	};

	inline BlockError::BlockError(Exception const& exception, BlockIdentifier const& runnableId) :
		m_Exception(exception),
		m_Runnable(runnableId)
	{
	}

	inline BlockIdentifier const& BlockError::getId() const
	{
		return m_Runnable;
	}

	inline Exception const& BlockError::getException() const
	{
		return m_Exception;
	}

}