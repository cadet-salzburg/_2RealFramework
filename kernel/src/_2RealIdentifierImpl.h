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

#include "_2RealIEntity.h"

#include <string>

namespace _2Real
{

	/**
	*	
	*/

	class IdentifierImpl
	{

		friend class Entities;

	public:

		/**
		*
		*/
		IdentifierImpl();

		/**
		*	
		*/
		IdentifierImpl(IdentifierImpl const& _src);

		/**
		*	
		*/
		IdentifierImpl& operator=(IdentifierImpl const& _src);

		/**
		*	
		*/
		~IdentifierImpl();

		/**
		*	
		*/
		bool operator==(IdentifierImpl const& _rhs) const;

		/**
		*	
		*/
		bool operator!=(IdentifierImpl const& _rhs) const;

		/**
		*
		*/
		bool operator<(IdentifierImpl const& _rhs) const;

		/**
		*
		*/
		bool operator<=(IdentifierImpl const& _rhs) const;

		/**
		*
		*/
		bool operator>(IdentifierImpl const& _rhs) const;

		/**
		*
		*/
		bool operator>=(IdentifierImpl const& _rhs) const;

		/**
		*	
		*/
		std::string const& strType() const;

		/**
		*	
		*/
		std::string const& name() const;

		/**
		*	
		*/
		unsigned int const& id() const;

		/**
		*	
		*/
		IEntity::eType const& type() const;

	private:

		/**
		*	
		*/
		IdentifierImpl(std::string const& _name, std::string const& _strType, IEntity::eType const& _type, unsigned int const& _id);

		/**
		*	orginal name given by application programmer
		*/
		std::string		m_Name;

		/**
		*	type
		*/
		IEntity::eType	m_Type;

		/**
		*	type as string
		*/
		std::string		m_Typename;

		/**
		*	unique id
		*/
		unsigned int	m_ID;

	};

}