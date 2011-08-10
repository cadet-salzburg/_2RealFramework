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
#include <vector>

#include "Poco/SharedPtr.h"

namespace _2Real
{

	class Metadata;

	typedef Poco::SharedPtr < Metadata > MetadataPtr;

	class Metadata
	{

		friend class MetadataReader;
		friend class OutputContainer;

	public:

		Metadata(std::string _name) : m_Name(_name) {}
		Metadata(const Metadata& _src);
		Metadata& operator= (const Metadata& _src);
		~Metadata();

		const Metadata* const father() const { return m_Father; }
		const std::string& name() const { return m_Name; }
		Data const& attributes() const { return m_Attributes; }
		
		template<typename T>
		const bool attribute(const std::vector<std::string>& _path, std::string _attribute, T& _result) const;

		template<typename T>
		const bool attribute(std::string _attribute, T& _result) const;

		const MetadataPtr subtree(const std::vector<std::string>& _path) const;
		const MetadataPtr child(std::string _name) const;

		Metadata* const father() { return m_Father; }

	protected:

		const bool insert(MetadataPtr& _metadata);
		const bool insert(const std::vector<std::string>& _path, MetadataPtr& _metadata);

		template<typename T>
		const bool setAttribute(std::string _attribute, const T& _value);

		template<typename T>
		const bool setAttribute(const std::vector<std::string>& _path, std::string _attribute, const T& _value);

	private:

		typedef std::map<std::string, MetadataPtr> MetadataMap;
		typedef std::pair<std::string, MetadataPtr> MetadataValue;

		std::string					m_Name;
		Data						m_Attributes;
		MetadataMap					m_Children;
		Metadata*					m_Father;

		void setFather(Metadata* _father) { m_Father = _father; }
		void setName(std::string _name) { m_Name = _name; }

		MetadataPtr subtree(const std::vector<std::string>& _path);
		MetadataPtr child(std::string _name);

	};

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
}