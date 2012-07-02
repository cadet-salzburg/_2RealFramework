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
		class BundleInternal;
		class FunctionBlock;
	//}

	namespace app
	{
		class BlockData
		{

			friend class _2Real::FunctionBlock;
			friend class _2Real::BundleInternal;

		public:

			// (?) are we going to keep this as a vector for sure, or
			// would it be wiser to apply a form of type erasure?
			// at this point, i don't think its worth the extra effort
			typedef std::vector< ParameterData >					Params;
			typedef std::vector< ParameterData >::iterator			ParamIterator;
			typedef std::vector< ParameterData >::const_iterator	ParamConstIterator;

			BlockData();

			std::string const&	getName() const;
			std::string const&	getDescription() const;
			std::string const&	getCategory() const;

			Params const&		getParameters() const;
			Params const&		getInlets() const;
			Params const&		getOutlets() const;

		private:

			std::string			m_Name;
			std::string			m_Description;
			std::string			m_Category;

			Params				m_Parameters;
			Params				m_Inlets;
			Params				m_Outlets;

		};
	}
}