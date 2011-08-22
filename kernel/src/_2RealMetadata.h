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

#include <map>
#include <string>
#include <vector>

namespace _2Real
{

	/**
	*
	*/

	class Metadata
	{

	public:

		/**
		*	
		*/
		typedef std::pair< const std::string, const std::string >	NamedAttribute;

		/**
		*	
		*/
		typedef std::map< const std::string, const std::string >	AttributeMap;

		/**
		*	
		*/
		Metadata(std::string const& _name, Metadata *const _father);

		/**
		*	
		*/
		Metadata(Metadata const& _src);

		/**
		*	
		*/
		Metadata& operator=(Metadata const& _src);

		/**
		*	
		*/
		~Metadata();

		/**
		*	
		*/
		const std::string& name() const;

		/**
		*	
		*/
		AttributeMap const& attributes() const;

		/**
		*	
		*/
		Metadata *const father();

		/**
		*	
		*/
		const Metadata *const father() const;

		/**
		*	
		*/
		Metadata *const child(std::string const& _name);

		/**
		*	
		*/
		const Metadata *const child(std::string const& _name) const;

		/**
		*
		*/
		Metadata *const subtree(std::vector< std::string > const& _path);
		
		/**
		*
		*/
		const Metadata *const subtree(std::vector< std::string > const& _path) const;

		/**
		*	inserts metadata into children
		*/
		const bool insert(Metadata *const _metadata);
		
		/**
		*	inserts metadata into children
		*/
		const bool insert(std::vector< std::string > const& _path, Metadata *const _metadata);

	private:

		/**
		*	
		*/
		typedef std::pair< std::string, Metadata * >	NamedChild;

		/**
		*	
		*/
		typedef std::map< std::string, Metadata * >		ChildMap;

		/**
		*	tag name
		*/
		std::string										m_Name;

		/**
		*	attributes
		*/
		AttributeMap									m_Attributes;

		/**
		*	children
		*/
		ChildMap										m_Children;

		/**
		*	father
		*/
		Metadata										*m_Father;

	};

}