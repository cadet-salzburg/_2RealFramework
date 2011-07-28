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
	class AbstractServiceVariable
	{

	public:

		AbstractServiceVariable(std::string _name) { m_OriginalName = _name; }
		std::string const& originalName() const { return m_OriginalName; }
		std::string const& frameworkName() const { return m_FrameworkName; }
		void setFrameworkName(std::string _name) { m_FrameworkName = _name; }
		
		virtual bool getFrom(const Data &_data) = 0;
		virtual void insertInto(Data &_data) const = 0;

	private:

		std::string			m_OriginalName;
		std::string			m_FrameworkName;

	};
}