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

#include "datatypes/_2RealFields.h"

#include <map>
#include <vector>
#include <string>

namespace _2Real
{
	class FieldDescriptor;
	class CustomType;
	class TypeRegistry;

	class TypeMetadata
	{

	public:

		TypeMetadata( std::string const& name, TypeRegistry const* reg );
		~TypeMetadata();
		void addField( std::string const& name, std::string const& type, FieldDescriptor const* desc );
		void getFields( Fields &fields ) const;
		std::string const& getName() const { return mName; }

	private:

		TypeMetadata( TypeMetadata const& other );
		TypeMetadata& operator=( TypeMetadata const& other );

		friend class CustomType;
		typedef std::map< std::string, FieldDescriptor const* >		Fields;
		Fields														mFields;
		std::string													mName;
		TypeRegistry												const* mRegistry;

	};
}