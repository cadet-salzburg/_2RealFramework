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

#include "_2RealConfigMetadata.h"

namespace _2Real
{
	ConfigMetadata::ConfigMetadata(ServiceName const& _name) : Metadata(_name)
	{
		m_SetupPath.push_back("setup params");
		m_InputPath.push_back("input params");
		m_OutputPath.push_back("output params");

		MetadataPtr setupMetadata(new Metadata("setup params"));
		MetadataPtr inputMetadata(new Metadata("input params"));
		MetadataPtr outputMetadata(new Metadata("output params"));
		
		Metadata::insert(setupMetadata);
		Metadata::insert(inputMetadata);
		Metadata::insert(outputMetadata);
	}

	ConfigMetadata::ConfigMetadata(const ConfigMetadata& _src) : Metadata(_src)
	{ 
		m_SetupPath = _src.m_SetupPath;
		m_InputPath = _src.m_InputPath;
		m_OutputPath = _src.m_OutputPath;
	}
		
	ConfigMetadata& ConfigMetadata::operator= (const ConfigMetadata& _src)
	{ 
		if (this == &_src)
		{
			return *this;
		}

		Metadata::operator=(_src);

		m_SetupPath = _src.m_SetupPath;
		m_InputPath = _src.m_InputPath;
		m_OutputPath = _src.m_OutputPath;

		return *this;
	}
		
	ConfigMetadata::~ConfigMetadata()
	{ 
	}

	Data const& ConfigMetadata::setupAttributes() const
	{
		const Metadata *metadata = Metadata::child("setup params");
		return metadata->attributes();
	}

	Data const& ConfigMetadata::intputAttributes() const
	{
		const Metadata *metadata = Metadata::child("input params");
		return metadata->attributes();
	}

	Data const& ConfigMetadata::outputAttributes() const
	{
		const Metadata *metadata = Metadata::child("output params");
		return metadata->attributes();
	}
}