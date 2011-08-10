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
	Metadata::Metadata(const Metadata& _src)
	{
		m_Name = _src.m_Name;
		m_Attributes = _src.m_Attributes;
		m_Father = _src.m_Father;
		m_Children = _src.m_Children;
	}

	Metadata& Metadata::operator= (const Metadata& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		m_Name = _src.m_Name;
		m_Attributes = _src.m_Attributes;
		m_Father = _src.m_Father;
		m_Children = _src.m_Children;
		
		return *this;
	}

	Metadata::~Metadata()
	{
	}

	const bool Metadata::insert(MetadataPtr& _metadata)
	{
		MetadataMap::iterator it = m_Children.find(_metadata->name());
		if (it != m_Children.end())
		{
			return false;
		}

		std::cout << "inserting child " << _metadata->name() << " into " << m_Name << std::endl;
		m_Children.insert(MetadataValue(_metadata->name(), _metadata));
		_metadata->setFather(this);
		return true;
	}

	const bool Metadata::insert(const std::vector<std::string>& _path, MetadataPtr& _metadata)
	{
		if (_path.empty())
		{
			return insert(_metadata);
		}

		std::vector<std::string> path = _path;
		MetadataMap::iterator it = m_Children.find(path.front());

		if (it == m_Children.end())
		{
			return false;
		}

		path.erase(path.begin());
		return it->second->insert(path, _metadata);
	}

	const MetadataPtr Metadata::subtree(const std::vector<std::string>& _path) const
	{
		if (_path.empty())
		{
			return MetadataPtr();
		}

		MetadataMap::const_iterator it = m_Children.find(_path.front());
		if (it == m_Children.end())
		{
			return MetadataPtr();
		}
		else if (_path.size() == 1)
		{
			return it->second;
		}

		std::vector<std::string> path = _path;
		path.erase(path.begin());
		return it->second->subtree(path);
	}

	MetadataPtr Metadata::subtree(const std::vector<std::string>& _path)
	{
		std::cout << "seeking subtree" << std::endl;

		if (_path.empty())
		{
			std::cout << "path empty" << std::endl;
			return MetadataPtr();
		}

		MetadataMap::iterator it = m_Children.find(_path.front());
		if (it == m_Children.end())
		{
			return MetadataPtr();
		}
		else if (_path.size() == 1)
		{
			std::cout << "found!" << std::endl;
			return it->second;
		}

		std::vector<std::string> path = _path;
		path.erase(path.begin());
		return it->second->subtree(path);
	}

	const MetadataPtr Metadata::child(std::string _name) const
	{
		MetadataMap::const_iterator it = m_Children.find(_name);
		if (it == m_Children.end())
		{
			return MetadataPtr();
		}
		return it->second;
	}

	MetadataPtr Metadata::child(std::string _name)
	{
		MetadataMap::iterator it = m_Children.find(_name);
		if (it == m_Children.end())
		{
			return MetadataPtr();
		}

		return it->second;
	}
}