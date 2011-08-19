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

#include "Poco/Any.h"
#include "Poco/SharedPtr.h"

namespace _2Real
{

	class ConfigMetadata;
	class IdentifierImpl;
	class Exception;

	/**
	*	
	*/
	typedef Poco::SharedPtr< Poco::Any >	SharedAnyPtr;

	class AbstractValue
	{

		friend class ServiceImpl;

	protected:

		/**
		*	
		*/
		AbstractValue();

		/**
		*	
		*/
		AbstractValue(std::string const& _name);

		/**
		*	
		*/
		AbstractValue(AbstractValue const& _src);

		/**
		*	
		*/
		AbstractValue& operator=(AbstractValue const& _src);

		/**
		*	
		*/
		~AbstractValue();

		/**
		*
		*/
		std::string const& name() const;

	private:

		/**
		*	name registered by plugin
		*/
		const std::string				m_Name;

	};

}