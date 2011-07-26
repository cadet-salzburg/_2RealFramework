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
	_2RealConfigMetadata::_2RealConfigMetadata(std::string _name) : _2RealMetadata(_name)
	{
		m_SetupPath.push_back("setup params");
		m_InputPath.push_back("input params");
		m_OutputPath.push_back("output params");

		_2RealMetadataPtr setupMetadata(new _2RealMetadata("setup params"));
		_2RealMetadataPtr inputMetadata(new _2RealMetadata("input params"));
		_2RealMetadataPtr outputMetadata(new _2RealMetadata("output params"));
		
		_2RealMetadata::insert(setupMetadata);
		_2RealMetadata::insert(inputMetadata);
		_2RealMetadata::insert(outputMetadata);
	}

	_2RealConfigMetadata::_2RealConfigMetadata(const _2RealConfigMetadata& _src) : _2RealMetadata(_src)
	{ 
		m_SetupPath = _src.m_SetupPath;
		m_InputPath = _src.m_InputPath;
		m_OutputPath = _src.m_OutputPath;
	}
		
	_2RealConfigMetadata& _2RealConfigMetadata::operator= (const _2RealConfigMetadata& _src)
	{ 
		if (this == &_src)
		{
			return *this;
		}

		_2RealMetadata::operator=(_src);

		m_SetupPath = _src.m_SetupPath;
		m_InputPath = _src.m_InputPath;
		m_OutputPath = _src.m_OutputPath;

		return *this;
	}
		
	_2RealConfigMetadata::~_2RealConfigMetadata()
	{ 
	}

}