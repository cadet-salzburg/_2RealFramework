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

#include "_2RealMetadata.h"

namespace _2Real
{

	Metadata::Metadata(std::string const& _name, Metadata *const _father) :
		m_Name(_name), m_Father(_father)
	{
	}

	Metadata::Metadata(Metadata const& _src) :
		m_Name(_src.m_Name), m_Attributes(_src.m_Attributes), m_Father(_src.m_Father)
	{
		//destructor of each child
		m_Children.clear();

		for (NamedMetadataMap::const_iterator it = _src.m_Children.begin(); it != _src.m_Children.end(); it++)
		{
			//recursively copy children
			Metadata tmp(*it->second);
			m_Children.insert(NamedChild(it->first, &tmp));
		}
	}

	Metadata& Metadata::operator=(Metadata const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		m_Father = _src.m_Father;
		m_Name = _src.m_Name;
		
		//attribs are shared ptrs, so no problem
		m_Attributes = _src.m_Attributes;

		//delete children
		m_Children.clear();

		for (NamedMetadataMap::const_iterator it = _src.m_Children.begin(); it != _src.m_Children.end(); it++)
		{
			//recursively copy children
			Metadata tmp(*it->second);
			m_Children.insert(NamedChild(it->first, &tmp));
		}
		
		return *this;
	}

	Metadata::~Metadata()
	{
		for (NamedMetadataMap::const_iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			//recursively destroy children
			delete it->second;
		}
	}

	const Metadata *const Metadata::father() const
	{
		return m_Father;
	}

	Metadata *const Metadata::father()
	{
		return m_Father;
	}

	std::string const& Metadata::name() const
	{
		return m_Name;
	}

	ReadableData const& Metadata::attributes() const
	{
		return m_Attributes;
	}

	Metadata *const Metadata::child(std::string const& _name)
	{
		NamedMetadataMap::iterator it = m_Children.find(_name);
		
		if (it == m_Children.end())
		{
			return NULL;
		}

		return it->second;
	}

	const Metadata *const Metadata::child(std::string const& _name) const
	{
		NamedMetadataMap::const_iterator it = m_Children.find(_name);
		
		if (it == m_Children.end())
		{
			return NULL;
		}

		return it->second;
	}

	const bool Metadata::insert(Metadata *const _metadata)
	{
		NamedMetadataMap::iterator it = m_Children.find(_metadata->name());
		
		if (it != m_Children.end())
		{
			return false;
		}

		m_Children.insert(NamedChild(_metadata->name(), _metadata));
		_metadata->m_Father = this;
		
		return true;
	}

	const bool Metadata::insert(std::vector< std::string > const& _path, Metadata *const _metadata)
	{
		if (_path.empty())
		{
			return insert(_metadata);
		}

		std::vector<std::string> path = _path;
		NamedMetadataMap::iterator it = m_Children.find(path.front());

		//means that the path defined by _path doesn't actually exist
		if (it == m_Children.end())
		{
			return false;
		}

		//erase first elem of path
		path.erase(path.begin());
		return it->second->insert(path, _metadata);
	}

	const Metadata *const Metadata::subtree(std::vector< std::string > const& _path) const
	{
		if (_path.empty())
		{
			return NULL;
		}

		NamedMetadataMap::const_iterator it = m_Children.find(_path.front());
		
		if (it == m_Children.end())
		{
			return NULL;
		}
		else if (_path.size() == 1)
		{
			return it->second;
		}

		std::vector< std::string > path = _path;
		path.erase(path.begin());
		return it->second->subtree(path);
	}

	Metadata *const Metadata::subtree(std::vector< std::string > const& _path)
	{
		if (_path.empty())
		{
			return NULL;
		}

		NamedMetadataMap::iterator it = m_Children.find(_path.front());
		
		if (it == m_Children.end())
		{
			return NULL;
		}
		else if (_path.size() == 1)
		{
			return it->second;
		}

		std::vector< std::string > path = _path;
		path.erase(path.begin());
		return it->second->subtree(path);
	}
}