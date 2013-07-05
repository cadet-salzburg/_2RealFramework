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

#include <string>
#include <vector>

namespace _2Real
{
	class Field;

	typedef std::vector< Field * > Fields;

	class Field
	{
	public:
		Field() {}
		virtual ~Field() {}
		virtual std::string getName() const = 0;
		virtual std::string getTypename() const = 0;
		virtual Fields getSubFields() const = 0;
		virtual void setName( std::string const& name ) = 0;
	};

	class SimpleField : public Field
	{
	public:
		std::string getName() const { return mName; }
		std::string getTypename() const { return mType; }
		Fields getSubFields() const { return Fields(); }
		void setName( std::string const& name ) { mName = name; }
	//private:
		std::string					mName;
		std::string					mType;
	};

	class ComplexField : public Field
	{
	public:
		std::string getName() const { return mName; }
		std::string getTypename() const { return mType; }
		Fields getSubFields() const { return mFields; }
		void setName( std::string const& name ) { mName = name; }
	//private:
		std::string					mName;
		std::string					mType;
		Fields						mFields;
	};
}
