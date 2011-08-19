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

#include "_2RealAnyValue.h"

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

		friend class ServiceImpl;

	public:

		/**
		*	registers a reference to an input variable in the framework
		*/
		template< typename T >
		void registerInputVariable(std::string const& _name, T &_var)
		{
			AbstractValue *value = new AnyValue< T >(_name, _var);
			registerInputVariable(value);
		}

		/**
		*	registers a reference to an output variable in the framework
		*/
		template< typename T >
		void registerOutputVariable(std::string const& _name, T &_var)
		{
			AbstractValue *value = new AnyValue< T >(_name, _var);
			registerOutputVariable(value);
		}

		/**
		*	retrieves the value of a setup parameter from the framework
		*/
		template< typename T >
		void getSetupParameter(std::string const& _name, T &_param)
		{
			AbstractValue *value = new AnyValue< T >(_name, _param);
			getSetupParameter(value);
		}
	
	private:

		/**
		*
		*/
		~ServiceContext();

		/**
		*
		*/
		ServiceContext(ServiceImpl *const _service);
		
		/**
		*
		*/
		ServiceContext(ServiceContext const& _src);
		
		/**
		*
		*/
		ServiceContext& operator=(ServiceContext const& _src);

		/**
		*	internal method for retrieving setup params
		*/
		void getSetupParameter(AbstractValue *const _var);
		
		/**
		*	internal method for registering input variables
		*/
		void registerInputVariable(AbstractValue *const _var);
		
		/**
		*	internal method for registering ouput variables
		*/
		void registerOutputVariable(AbstractValue *const _param);

		/**
		*	ptr to service container
		*/
		ServiceImpl			*m_Container;

	};

}