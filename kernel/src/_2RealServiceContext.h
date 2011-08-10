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

#include "_2RealTypedefs.h"

#include "_2RealAbstractService.h"

#include <string>

namespace _2Real
{

	class ServiceContext
	{

		friend class AbstractService;

	public:

		/**
		*
		*/
		template< typename T >
		void registerInputVariable(Variable _name, T &_var)
		{
			m_ServicePtr->addInputVariable< T >(_name, _var);
		}

		/**
		*
		*/
		template< typename T >
		void registerOutputVariable(Variable _name, T &_var)
		{
			m_ServicePtr->addInputVariable< T >(_name, _var);
		}

		/**
		*
		*/
		template< typename T >
		void registerSetupParameter(Variable _name, T &_param)
		{
			m_ServicePtr->addSetupParameter< T >(_name, _var);
		}
	
	private:

		/**
		*
		*/
		~ServiceContext();

		/**
		*
		*/
		ServiceContext(AbstractService *const _service);
		
		/**
		*
		*/
		ServiceContext(const ServiceContext& _src);
		
		/**
		*
		*/
		ServiceContext& operator=(const ServiceContext& _src);

		/**
		*
		*/
		AbstractService			*m_ServicePtr;

	};

}