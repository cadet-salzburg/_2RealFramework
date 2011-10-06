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

#include "_2RealEntity.h"

#include <string>

namespace _2Real
{

	/**
	*
	*/

	class Parameter : public Entity
	{

	public:

		/**
		*	
		*/
		Parameter(Id *const _id, std::string const& _type);

		/**
		*	
		*/
		Parameter(Parameter const& _src);

		/**
		*	
		*/
		Parameter& operator=(Parameter const& _src);

		/**
		*	
		*/
		virtual ~Parameter();

		/**
		*	true if value has been set at some point
		*/
		bool const& isInitialized() const;

		/**
		*	typename as string
		*/
		std::string const& datatype() const;

	protected:

		/**
		*	
		*/
		bool						m_bIsInitialized;

		/**
		*
		*/
		const std::string			m_Typename;

	};

}