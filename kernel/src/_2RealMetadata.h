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

#include <typeinfo.h>
#include <string>

namespace _2Real
{

	class PluginMetadata;

	class Metadata
	{

	public:

		Metadata(PluginMetadata &metadata);

		/**
		*	sets plugin description
		*
		*	@param description:		plugin's description
		*/
		void setDescription(std::string const& description);

		/**
		*	sets plugin version
		*
		*	@param major			
		*	@param minor			
		*	@param revision			
		*/
		void setVersion(unsigned int major, unsigned int minor, unsigned int revision);

		/**
		*	sets plugin author
		*
		*	@param name:			plugin's author
		*/
		void setAuthor(std::string const& author);

		/**
		*	sets plugin contact address
		*
		*	@param contact:			plugin's contact address
		*/
		void setContact(std::string const& contact);

		/**
		*	add setup parameter
		*
		*	@param setupName:		parameter's name
		*	@throw					AlreadyExistsException, InvalidTypeException
		*/
		template< typename Datatype >
		void addSetupParameter(std::string const& setupName)
		{
			addSetupParameterByType(setupName, typeid(Datatype).name());
		}

		/**
		*	add a service
		*
		*	@param serviceName:		service's name
		*	@throw					AlreadyExistsException
		*/
		void addService(std::string const& serviceName);

		/**
		*	set service description
		*
		*	@param serviceName		service's name
		*	@param description		service's description
		*	@throw					NotFoundException
		*/
		void setDescription(std::string const& serviceName, std::string const& description);

		/**
		*	add service setup parameter
		*
		*	@param serviceName		service's name
		*	@param setupName		parameter's name
		*	@throw					NotFoundException(service), AlreadyExistsException(parameter), InvalidTypeException
		*/
		template< typename Datatype >
		void addSetupParameter(std::string const& serviceName, std::string const& setupName)
		{
			addSetupParameterByType(serviceName, setupName, typeid(Datatype).name());
		}

		/**
		*	add service input slot
		*
		*	@param serviceName		service's name
		*	@param inputName		slot's name
		*	@throw					NotFoundException(service), AlreadyExistsException(slot), InvalidTypeException
		*/
		template< typename Datatype >
		void addInputSlot(std::string const& serviceName, std::string const& inputName)
		{
			addInputSlotByType(serviceName, inputName, typeid(Datatype).name());
		}

		/**
		*	add service output slot
		*
		*	@param serviceName		service's name
		*	@param outputName		slot's name
		*	@throw					NotFoundException(service), AlreadyExistsException(slot), InvalidTypeException
		*/
		template< typename Datatype >
		void addOutputSlot(std::string const& serviceName, std::string const& outputName)
		{
			addOutputSlotByType(serviceName, outputName, typeid(Datatype).name());
		}

	private:

		void				addSetupParameterByType(std::string const& setupName, std::string const& type);
		void				addSetupParameterByType(std::string const& serviceName, std::string const& setupName, std::string const& type);
		void				addInputSlotByType(std::string const& serviceName, std::string const& inputName, std::string const& type);
		void				addOutputSlotByType(std::string const& serviceName, std::string const& outputName, std::string const& type);

		PluginMetadata		&m_Impl;

	};

}