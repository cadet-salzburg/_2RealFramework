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
#include "_2RealEngineData.h"

namespace _2Real
{

	class Inlet;

	class InletHandle : public ParameterHandle
	{

	public:

		InletHandle();
		InletHandle( Inlet &slot );
		InletHandle( InletHandle const& src );
		InletHandle& operator=( InletHandle const& src );

		template< typename Datatype >
		Datatype const& data()
		{
			if (!m_Input)
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			std::shared_ptr< Datatype > ptr = extractFrom< Datatype >( current() );
			return *ptr.get();
		}

		/**
		*	maybe the naming of this is clearer:
		*	returns a const ref to the actual input data; no copying necessary.
		*	of course, one can only read this.
		*/
		template< typename Datatype >
		Datatype const& getReadableRef()
		{
			if (!m_Input)
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			std::shared_ptr< Datatype > ptr = extractFrom< Datatype >(current());
			return *ptr.get();
		}

		/**
		*	returns a shared pointer to a copy of the data. this can be written
		*	as well as read, but it makes a copy a copy
		*/
		template< typename Datatype >
		std::shared_ptr< Datatype > getWriteableCopy()
		{
			if (!m_Input)
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			std::shared_ptr< Datatype > ptr = extractFrom< Datatype >(current());
			return std::shared_ptr< Datatype >(new Datatype(*ptr.get()));
		}

	private:

		const EngineData			current();
		Inlet						*m_Input;

	};

}