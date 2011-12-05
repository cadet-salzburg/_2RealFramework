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

namespace _2Real
{

	class OutputSlot;
	class EngineData;

	class OutputHandle : public ParameterHandle
	{

	public:

		OutputHandle();
		OutputHandle(OutputSlot *slot);
		OutputHandle(OutputHandle const& src);
		OutputHandle& operator=(OutputHandle const& src);
		~OutputHandle();

		template< typename Datatype >
		Datatype & data()
		{
			if (!m_Output)
			{
				throw BadHandleException(m_Name);
			}

			Poco::SharedPtr< Datatype > ptr = Extract< Datatype >(data());
			return *ptr.get();
		}

	private:

		EngineData			data();
		OutputSlot			*m_Output;

	};

}