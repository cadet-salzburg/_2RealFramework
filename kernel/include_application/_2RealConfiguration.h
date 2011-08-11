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

#include "_2RealServiceVariable.h"
#include "_2RealAbstractServiceVariable.h"

namespace _2Real
{

	/**
	*
	*/

	/**
	*
	*/
	enum eContainerType
	{
		SEQUENCE			= 0x00000000,
		SYNCHRONIZATION		= 0x00000001,
		MUTEX				= 0x00000002,
	};

	class Context;
	class OutputContainer;

	class Configuration
	{

		friend class Framework;

	public:

		/**
		*
		*/
		const bool beginConfiguration();

		/**
		*
		*/
		const bool endConfiguration();

		/**
		*
		*/
		const bool beginGroup(eContainerType const& _type);

		/**
		*
		*/
		const bool endGroup();

		/**
		*
		*/
		const bool beginServiceConfiguration(std::string const& _name, std::string const& _plugin);

		/**
		*
		*/
		const bool endServiceConfiguration();

		/**
		*
		*/
		template< typename T >
		const bool configureSetupParameter(std::string const& _name, T const& _value)
		{
			AbstractServiceVariable *param = new ServiceVariable< T >(_name, _value);
			return configureSetupParameter(param);
		}

		/**
		*
		*/
		const Variable configureOutputParameter(std::string const& _name);

		/**
		*
		*/
		const bool configureInputParameter(std::string const& _name, Variable const& _var);

	private:

		/**
		*
		*/
		Configuration(OutputContainer *const _container);

		/**
		*
		*/
		Configuration(Configuration const& _src);

		/**
		*
		*/
		Configuration& operator=(Configuration const& _src);

		/**
		*
		*/
		~Configuration();

		/*
		*/
		const bool configureSetupParameter(AbstractServiceVariable *_param);

		/**
		*
		*/
		OutputContainer			*m_ContainerPtr;

	};

}