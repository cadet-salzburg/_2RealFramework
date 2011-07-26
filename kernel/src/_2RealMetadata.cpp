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
	_2RealMetadata::_2RealMetadata(const _2RealMetadata& _src)
	{
		m_Name = _src.m_Name;
		m_Attributes = _src.m_Attributes;
		m_Father = _src.m_Father;
		m_Children = _src.m_Children;
	}

	_2RealMetadata& _2RealMetadata::operator= (const _2RealMetadata& _src)
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

	_2RealMetadata::~_2RealMetadata()
	{
	}

	const bool _2RealMetadata::insert(_2RealMetadataPtr& _metadata)
	{
		MetadataMap::iterator it = m_Children.find(_metadata->name());
		if (it != m_Children.end())
		{
			return false;
		}

		m_Children.insert(Metadata(_metadata->name(), _metadata));
		_metadata->setFather(this);
		return true;
	}

	const bool _2RealMetadata::insert(const std::vector<std::string>& _path, _2RealMetadataPtr& _metadata)
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

	const _2RealMetadata* const _2RealMetadata::subtree(const std::vector<std::string>& _path) const
	{
		if (_path.empty())
		{
			return NULL;
		}

		MetadataMap::const_iterator it = m_Children.find(_path.front());
		if (it == m_Children.end())
		{
			return NULL;
		}
		else if (_path.size() == 1)
		{
			return it->second.get();
		}

		std::vector<std::string> path = _path;
		path.erase(path.begin());
		return it->second->subtree(path);
	}

	_2RealMetadata* _2RealMetadata::subtree(const std::vector<std::string>& _path)
	{
		if (_path.empty())
		{
			return NULL;
		}

		MetadataMap::iterator it = m_Children.find(_path.front());
		if (it == m_Children.end())
		{
			return NULL;
		}
		else if (_path.size() == 1)
		{
			return it->second.get();
		}

		std::vector<std::string> path = _path;
		path.erase(path.begin());
		return it->second->subtree(path);
	}

	const _2RealMetadata* const _2RealMetadata::child(std::string _name) const
	{
		MetadataMap::const_iterator it = m_Children.find(_name);
		if (it == m_Children.end())
		{
			return NULL;
		}

		return it->second.get();
	}

	_2RealMetadata* _2RealMetadata::child(std::string _name)
	{
		MetadataMap::iterator it = m_Children.find(_name);
		if (it == m_Children.end())
		{
			return NULL;
		}

		return it->second.get();
	}
}