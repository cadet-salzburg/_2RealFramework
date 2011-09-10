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

	public:

		/**
		*	
		*/
		IdentifierImpl(std::string const& _name, std::string const& _strType, std::string const& _info, Entity::eType const& _type, unsigned int const& _id);

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
		std::string const& info() const;

		/**
		*	
		*/
		unsigned int const& id() const;

		/**
		*	
		*/
		Entity::eType const& type() const;

		/**
		*
		*/
		void retain();

		/**
		*
		*/
		void release();

		/**
		*
		*/
		void setInfo(std::string const& _info);

	private:

		/**
		*	orginal name given by application programmer
		*/
		const std::string			m_Name;

		/**
		*	type as string
		*/
		const std::string			m_Typename;

		/**
		*	info
		*/
		std::string					m_Info;

		/**
		*	type
		*/
		const Entity::eType			m_Type;

		/**
		*	unique id
		*/
		const unsigned int			m_ID;

		/**
		*	ref count
		*/
		unsigned int				m_iRefCount;

	};

}