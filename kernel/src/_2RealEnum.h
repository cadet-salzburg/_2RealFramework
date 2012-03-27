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

#include <sstream>
#include <string>
#include <map>
#include <list>

namespace _2Real
{

	typedef std::map< std::string, std::string > EnumMap;
	typedef std::list< std::string > StringList;

	class Strings
	{

	public:

		Strings(std::string const& value);
		Strings& operator()(std::string const& name);
		operator StringList const& () const;

	private:

		StringList	m_List;

	};

	/**
	*	helper obj for enumeration initialization
	*/
	class Enums
	{

	public:

		/**
		*	the first (key, value) pair is immediately inserted into the map
		*/
		Enums(std::string const& name, std::string const& value);

		/**
		*	to add additional (key, value) pairs, use the subscript operator
		*/
		Enums& operator()(std::string const& name, std::string const& value);

		/**
		*	let's cast to a map, yay!
		*/
		operator EnumMap const& () const;

	private:

		EnumMap	m_Map;

	};

	/**
	*	the actual enumeration datatype
	*/
	class Enumeration
	{

	public:

		Enumeration();
		Enumeration(EnumMap const& enums, std::string const& undefinedVal);
		Enumeration(Enumeration const& src);
		std::string& operator[](std::string const& key);
		std::string const& valueFor(std::string const& key) const;

		void setValues(StringList values);
		void setEnums(StringList const& enums);

		friend std::ostream& operator<<(std::ostream& out, Enumeration const& e);
		friend std::istream& operator>>(std::istream& in, Enumeration &e);

		void writeTo(std::ostream& out) const;
		void readFrom(std::istream& in);

	private:

		EnumMap			m_Content;
		std::string		m_Undefined;

	};

}