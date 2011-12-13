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

#include "_2RealParameter.h"
#include "_2RealParameterMetadata.h"

namespace _2Real
{

	Parameter::Parameter(ParameterMetadata const& metadata) :
		m_Name(metadata.getName()),
		m_Datatype(metadata.getType()),
		m_Keyword(metadata.getKeyword())
	{
		if (metadata.hasDefaultValue())
		{
			m_IsInitialized = true;
		}
		else
		{
			m_IsInitialized = false;
		}
	}

	inline Parameter::~Parameter()
	{
	}

}