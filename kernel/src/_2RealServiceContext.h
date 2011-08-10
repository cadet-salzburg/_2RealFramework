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

#include "_2RealServiceParameter.h"
#include "_2RealServiceVariable.h"

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
			AbstractServiceVariable *var = new ServiceVariable< T >(_name, _var);
			registerInputVariable(var);
		}

		/**
		*
		*/
		template< typename T >
		void registerOutputVariable(Variable _name, T &_var)
		{
			AbstractServiceVariable *var = new ServiceVariable< T >(_name, _var);
			registerOutputVariable(var);
		}

		/**
		*
		*/
		template< typename T >
		void registerSetupParameter(Variable _name, T &_param)
		{
			AbstractServiceVariable *param = new ServiceParameter< T >(_name, _param);
			registerSetupParameter(param);
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
		void registerSetupParameter(AbstractServiceVariable *_param);
		
		/**
		*
		*/
		void registerInputVariable(AbstractServiceVariable *_var);
		
		/**
		*
		*/
		void registerOutputVariable(AbstractServiceVariable *_var);

		/**
		*
		*/
		AbstractService			*m_ServicePtr;

	};

}