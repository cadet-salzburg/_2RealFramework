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

#include "app/_2RealParameterData.h"

#include <vector>
#include <string>

namespace _2Real
{
	//namespace engine
	//{
		class Bundle;
		template< typename T > class FunctionBlock;
	//}

	namespace app
	{
		class BlockInfo
		{

			template< typename T >
			friend class _2Real::FunctionBlock;
			friend class _2Real::Bundle;

		public:

			typedef std::vector< ParameterInfo >					ParameterInfos;
			typedef std::vector< ParameterInfo >::iterator			ParameterInfoIterator;
			typedef std::vector< ParameterInfo >::const_iterator	ParameterInfoConstIterator;

			BlockInfo();

			std::string const&			getName() const;
			std::string const&			getDescription() const;
			std::string const&			getCategory() const;

			ParameterInfos const&		getParameters() const;
			ParameterInfos const&		getInlets() const;
			ParameterInfos const&		getOutlets() const; 

		private:

			std::string			m_Name;
			std::string			m_Description;
			std::string			m_Category;

			ParameterInfos		m_Parameters;
			ParameterInfos		m_Inlets;
			ParameterInfos		m_Outlets;

		};
	}
}