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

#include "_2RealMetadata.h"
//#include "_2RealData.h"
//
//#include <string>
//#include <vector>
//#include <list>

namespace _2Real
{

	/**
	*
	*/

	class ConfigurationData : public Metadata
	{

	//////	friend class OutputContainer;

	//////public:

	//////	/**
	//////	*	sets value of abstract service variable
	//////	*/
	//////	const bool configureValue(AbstractServiceVariable *const _var) const;

	//////	/**
	//////	*	sets framework given name for a variable
	//////	*/
	//////	const bool configureName(AbstractServiceVariable *const _var) const;

	//////private:

	//////	/**
	//////	*
	//////	*/
	//////	ProductionTreeData(std::string const& _name, ProductionTreeData *const _father);
	//////	
	//////	/**
	//////	*
	//////	*/
	//////	ProductionTreeData(ProductionTreeData const&_src);
	//////	
	//////	/**
	//////	*
	//////	*/
	//////	ProductionTreeData& operator= (ProductionTreeData const& _src);
	//////	
	//////	/**
	//////	*
	//////	*/
	//////	~ProductionTreeData();

	//////	/**
	//////	*
	//////	*/
	//////	VariableData			m_SetupValues;

	//////	const bool addToValues(AbstractServiceVariable const *const _var);

	//////	const bool addToVariables(AbstractServiceVariable const *const _var);

	};

}