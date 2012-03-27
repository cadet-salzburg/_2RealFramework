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

#include "_2RealEnum.h"
#include "_2RealHelpers.h"

namespace _2Real
{

	Strings::Strings(std::string const& value)
	{
		m_List.push_back(value);
	}

	Strings& Strings::operator()(std::string const& value)
	{
		m_List.push_back(value);
		return *this;
	}

	Strings::operator StringList const&() const
	{
		return m_List;
	}

	Enums::Enums(std::string const& name, std::string const& defaultVal)
	{
		m_Map[name] = defaultVal;
	}

	Enums& Enums::operator()(std::string const& name, std::string const& defaultVal)
	{
		m_Map[name] = defaultVal;
		return *this;
	}

	Enums::operator EnumMap const&() const
	{
		return m_Map;
	}

	Enumeration::Enumeration() :
		m_Content()
	{
	}

	Enumeration::Enumeration(EnumMap const& enums, std::string const& undefinedVal) :
		m_Content(enums),
		m_Undefined(undefinedVal)
	{
	}

	Enumeration::Enumeration(Enumeration const& src) :
		m_Content(src.m_Content),
		m_Undefined(src.m_Undefined)
	{
	}

	std::string& Enumeration::operator[](std::string const& key)
	{
		return m_Content[key];
	}

	std::string const& Enumeration::valueFor(std::string const& key) const
	{
		if (m_Content.find(key) != m_Content.end())
		{
			return m_Content.at(key);
		}

		else return m_Undefined;
	}

	void Enumeration::setEnums(StringList const& enums)
	{
		m_Content.clear();

		for (StringList::const_iterator it = enums.begin(); it != enums.end(); ++it)
		{
			m_Content.insert(make_pair(*it, m_Undefined));
		}
	}

	void Enumeration::setValues(StringList values)
	{
		for (EnumMap::iterator it = m_Content.begin(); it != m_Content.end(); ++it)
		{
			if (values.empty())
			{
				break;
			}

			it->second = values.front();
			values.pop_front();
		}
	}

	void Enumeration::writeTo(std::ostream& out) const
	{
		out << m_Content;
	}

	void Enumeration::readFrom(std::istream& in)
	{
		//TODO
		//in >> m_Content;
	}

	std::ostream& operator<<(std::ostream& out, Enumeration const& e)
	{
		e.writeTo(out);
		return out;
	}

	std::istream& operator>>(std::istream& in, Enumeration &e)
	{
		e.readFrom(in);
		return in;
	}

}