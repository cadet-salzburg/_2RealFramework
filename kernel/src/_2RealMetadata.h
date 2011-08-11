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

#include "_2RealData.h"

#include <string>
#include <map>
#include <vector>

namespace _2Real
{

	class Metadata
	{

	public:

		Metadata(std::string const& _name, Metadata *const _father);
		Metadata(Metadata const& _src);
		Metadata& operator=(Metadata const& _src);
		~Metadata();

		const std::string& name() const;
		ReadableData const& attributes() const;

		Metadata *const father();
		const Metadata *const father() const;

		Metadata *const child(std::string const& _name);
		const Metadata *const child(std::string const& _name) const;

		Metadata *const subtree(std::vector< std::string > const& _path);
		const Metadata *const subtree(std::vector< std::string > const& _path) const;

		const bool insert(Metadata *const _metadata);
		const bool insert(std::vector< std::string > const& _path, Metadata *const _metadata);

		//template<typename T>
		//const bool attribute(const std::vector<std::string>& _path, std::string _attribute, T& _result) const;

		//template<typename T>
		//const bool attribute(std::string _attribute, T& _result) const;

		//template< typename T >
		//const bool setAttribute(std::string const& _attribute, T const& _value);

		//template< typename T >
		//const bool setAttribute(std::vector< std::string > const& _path, std::string const& _attribute, T const& _value);

	private:

		typedef std::map< std::string, Metadata * >		NamedMetadataMap;
		typedef std::pair< std::string, Metadata * >	NamedChild;

		/**
		*	tag name
		*/
		std::string										m_Name;

		/**
		*	attributes - key : string
		*/
		ReadableData									m_Attributes;
		
		/**
		*	children
		*/
		NamedMetadataMap								m_Children;
		
		/**
		*	father
		*/
		Metadata*										m_Father;

	};

	/*
	template< typename T >
	const bool Metadata::attribute(const std::vector<std::string>& _path, std::string _attribute, T& _value) const
	{
		const MetadataPtr sub = subtree(_path);
		if (!sub.isNull() && sub->attribute< T >(_attribute, _value))
		{
			return true;
		}

		return false;
	}

	template< typename T >
	const bool Metadata::attribute(std::string _attribute, T& _result) const
	{	
		return m_Attributes.get< T >(_attribute, _result);
	}

	template<typename T>
	const bool Metadata::setAttribute(const std::vector<std::string>& _path, std::string _attribute, const T& _value)
	{
		//std::cout << "set attrib: " << _path[0] << " " << _attribute << std::endl;
		MetadataPtr sub = subtree(_path);
		if (!sub.isNull())
		{
			//std::cout << m_Name << " " << sub->name() << std::endl;
			if (sub->setAttribute<T>(_attribute, _value))
			{
				return true;
			}
		}
		
		return false;
	}

	template<typename T>
	const bool Metadata::setAttribute(std::string _attribute, const T& _value)
	{
		//std::cout << "in subtree "  << m_Name << " " << m_Attributes.size() << std::endl;
		
		m_Attributes.remove(_attribute);
		m_Attributes.insert< T >(_attribute, _value);
		return true;
	}
	*/
}