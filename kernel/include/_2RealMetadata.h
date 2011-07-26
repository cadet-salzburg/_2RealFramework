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

#include "_2RealFramework.h"
#include "_2RealData.h"

namespace _2Real
{
	class _2RealMetadata
	{

	public:

		_2RealMetadata(std::string _name) : m_Name(_name) {}
		_2RealMetadata(const _2RealMetadata& _src);
		_2RealMetadata& operator= (const _2RealMetadata& _src);
		~_2RealMetadata();

		const _2RealMetadata* const father() const { return m_Father; }
		const std::string& name() const { return m_Name; }
		
		template<typename T>
		const bool attribute(const std::vector<std::string>& _path, std::string _attribute, T& _result) const;

		template<typename T>
		const bool attribute(std::string _attribute, T& _result) const;

		const _2RealMetadata* const subtree(const std::vector<std::string>& _path) const;
		const _2RealMetadata* const child(std::string _name) const;

	protected:

		const bool insert(_2RealMetadataPtr& _metadata);
		const bool insert(const std::vector<std::string>& _path, _2RealMetadataPtr& _metadata);

		template<typename T>
		const bool setAttribute(std::string _attribute, const T& _value);

		template<typename T>
		const bool setAttribute(const std::vector<std::string>& _path, std::string _attribute, const T& _value);

	private:

		typedef std::map<std::string, _2RealMetadataPtr> MetadataMap;
		typedef std::pair<std::string, _2RealMetadataPtr> Metadata;

		std::string						m_Name;
		_2RealData						m_Attributes;
		MetadataMap						m_Children;
		_2RealMetadata*					m_Father;

		void setFather(_2RealMetadata* _father) { m_Father = _father; }
		void setName(std::string _name) { m_Name = _name; }

		_2RealMetadata* const father() { return m_Father; }
		_2RealMetadata* subtree(const std::vector<std::string>& _path);
		_2RealMetadata* child(std::string _name);

		friend class _2RealMetadataReader;
		friend class _2RealContext; 
	};

	template<typename T>
	const bool _2RealMetadata::attribute(const std::vector<std::string>& _path, std::string _attribute, T& _value) const
	{
		const _2RealMetadata* sub = subtree(_path);
		if (sub != NULL && sub->attribute<T>(_attribute, _value))
		{
			return true;
		}

		return false;
	}

	template<typename T>
	const bool _2RealMetadata::attribute(std::string _attribute, T& _result) const
	{	
		return m_Attributes.get<T>(_attribute, _result);
	}

	template<typename T>
	const bool _2RealMetadata::setAttribute(const std::vector<std::string>& _path, std::string _attribute, const T& _value)
	{
		_2RealMetadata* sub = subtree(_path);
		if (sub != NULL)
		{
			if (sub->setAttribute<T>(_attribute, _value))
			{
				return true;
			}
		}
		
		return false;
	}

	template<typename T>
	const bool _2RealMetadata::setAttribute(std::string _attribute, const T& _value)
	{
		m_Attributes.remove(_attribute);
		m_Attributes.insert<T>(_attribute, _value);
		return true;
	}
}