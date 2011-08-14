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

//included because of eContainerType
#include "_2RealTypedefs.h"
#include "_2RealAbstractContainer.h"

namespace _2Real
{

	/**
	*
	*/

	class ProductionContainer : public AbstractContainer
	{

	public:

		ProductionContainer(ContainerName const& _name);
		ProductionContainer(ProductionContainer const& _src);
		ProductionContainer& operator=(ProductionContainer const& _src);
		~ProductionContainer();

		/**
		*	functions inherited from IContainer
		*/

		/**
		*
		*/
		void configure(ConfigurationData *const _dataPtr) throw(...);

		/**
		*	function inherited from Poco::Runnable
		*/

		/**
		*
		*/
		void run() throw(...);

		/**
		*	functions inherited from IService
		*/

		/**
		*
		*/
		void update() throw(...);

		/**
		*
		*/
		void shutdown() throw(...);

		/**
		*
		*/
		void addContainer(eContainerType const& _type);

		/**
		*
		*/
		eContainerType const& type();

		/**
		*
		*/
		void setType(eContainerType const& _type);

	private:

		typedef std::list< AbstractContainer * >	ContainerList;

		/**
		*	child containers
		*/
		ContainerList								m_Children;

		eContainerType								m_Type;

	};

}