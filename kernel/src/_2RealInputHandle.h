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

#include <sstream>

namespace _2Real
{

	class InputSlot;
	class EngineData;

	class InputHandle : public ParameterHandle
	{

	public:

		InputHandle();
		InputHandle(InputSlot const& slot);
		InputHandle(InputHandle const& src);
		InputHandle& operator=(InputHandle const& src);

		template< typename Datatype >
		Datatype const& data()
		{
			if (!m_Input)
			{
				std::ostringstream msg;
				msg << "input handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			try
			{
				Poco::SharedPtr< Datatype > ptr = Extract< Datatype >(newest());
				return *ptr.get();
			}
			catch (Exception &e)
			{
				throw e;
			}
		}

	private:

		EngineData		oldest();
		EngineData		newest();

		InputSlot		const* m_Input;

	};

}