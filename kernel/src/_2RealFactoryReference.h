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
#include "_2RealServiceMetadata.h"

#include <string>

namespace _2Real
{

	/**
	*
	*/

	class IService;

	class FactoryReference : public Entity
	{

	public:

		/**
		*	service factory function
		*/
		typedef IService *const (*const ServiceCreator)(void);

		/**
		*
		*/
		FactoryReference(std::string const& _name, unsigned int const& _pluginID, ServiceCreator _creator, ServiceMetadata const& _metadata, IdentifierImpl *const _id);

		/**
		*
		*/
		FactoryReference(FactoryReference const& _src) throw(...);

		/**
		*
		*/
		FactoryReference& operator=(FactoryReference const& _src) throw(...);

		/**
		*
		*/
		~FactoryReference();

		/**
		*	plugin id
		*/
		unsigned int const& plugin() const;

		/**
		*	metadata
		*/
		ServiceMetadata const& metadata() const;

		/**
		*	name
		*/
		std::string const& name() const;

		/**
		*	creates service obj, or returns singleton instance
		*/
		IService *const create();

	private:

		/**
		*
		*/
		const unsigned int		m_Plugin;

		/**
		*
		*/
		const ServiceMetadata	m_Metadata;

		/**
		*
		*/
		const ServiceCreator	m_Creator;

		/**
		*	
		*/
		const std::string		m_Name;

	};

}