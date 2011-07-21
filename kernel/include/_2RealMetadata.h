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

#include "_2RealFramework.h"

namespace _2Real
{
	class _2RealMetaData
	{

	public:

		_2RealMetaData();
		_2RealMetaData(const _2RealMetaData& _src);
		_2RealMetaData& operator= (const _2RealMetaData& _src);

		void set(std::string, std::string);
		const std::string get(std::string);
		
	private:
			
		std::map<std::string, std::string> m_Data;

	};
}