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

#include "_2RealServiceVariable.h"

#include <string>

namespace _2Real
{

	/**
	*	this class offers a means of communication with the framework
	*	during its setup method, a user defined service can retrieve the value of its setup params
	*	as well as register references to its input & output variables
	*/

	class ServiceContext
	{

		friend class InternalService;

	public:

		/**
		*	registers a reference to an input variable in the framework
		*/
		template< typename T >
		const bool registerInputVariable(std::string const& _name, T &_var)
		{
			AbstractServiceVariable *var = new ServiceVariable< T >(_name, _var);
			return registerInputVariable(var);
		}

		/**
		*	registers a reference to an output variable in the framework
		*/
		template< typename T >
		const bool registerOutputVariable(std::string const& _name, T &_var)
		{
			AbstractServiceVariable *var = new ServiceVariable< T >(_name, _var);
			return registerOutputVariable(var);
		}

		/**
		*	retrieves the value of a setup parameter from the framework
		*/
		template< typename T >
		const bool getSetupParameter(std::string const& _name, T &_param)
		{
			AbstractServiceVariable *param = new ServiceVariable< T >(_name, _param);
			return getSetupParameter(param);
		}
	
	private:

		/**
		*	private dtor
		*/
		~ServiceContext();

		/**
		*	private ctor
		*/
		ServiceContext(InternalService *const _service);
		
		/**
		*	private copy ctor
		*/
		ServiceContext(const ServiceContext& _src);
		
		/**
		*	private assignment operator
		*/
		ServiceContext& operator=(const ServiceContext& _src);

		/**
		*	internal method for retrieving setup params
		*/
		const bool getSetupParameter(AbstractServiceVariable *_param);
		
		/**
		*	internal method for registering input variables
		*/
		const bool registerInputVariable(AbstractServiceVariable *_var);
		
		/**
		*	internal method for registering ouput variables
		*/
		const bool registerOutputVariable(AbstractServiceVariable *_var);

		/**
		*	ptr to internal service
		*/
		InternalService			*m_ServicePtr;

	};

}