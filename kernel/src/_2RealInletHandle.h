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

#include "_2RealParameterHandle.h"

#include "Poco/SharedPtr.h"

#include <sstream>

namespace _2Real
{

	class Inlet;
	class EngineData;

	class InletHandle : public ParameterHandle
	{

	public:

		InletHandle();
		InletHandle(Inlet &slot);
		InletHandle(InletHandle const& src);
		InletHandle& operator=(InletHandle const& src);

		template< typename Datatype >
		Datatype const& data()
		{
			if (!m_Input)
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			Poco::SharedPtr< Datatype > ptr = Extract< Datatype >(newest());
			return *ptr.get();
		}

		/**
		*	maybe the naming of this is clearer:
		*	returns a const ref to the actual input data; no copying necessary.
		*	of course, one can only read this.
		*/
		template< typename DataType >
		DataType const& getReadableRef()
		{
			if (!m_Input)
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			Poco::SharedPtr< DataType > ptr = Extract< DataType >(newest());
			return *ptr.get();
		}

		/**
		*	returns a shared pointer to a copy of the data. this can be written
		*	as well as read, but it makes a copy a copy
		*/
		template< typename DataType >
		Poco::SharedPtr< DataType > getWriteableCopy()
		{
			if (!m_Input)
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			Poco::SharedPtr< Datatype > ptr = Extract< Datatype >(newest());
			return Poco::SharedPtr< DataType >(new DataType(*ptr.get()));
		}

	private:

		const EngineData				newest();
		Inlet						*m_Input;

	};

}