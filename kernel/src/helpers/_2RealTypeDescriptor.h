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

#include "datatypes/_2RealType.h"
#include "datatypes/_2RealTypeCategory.h"

#include <string>

#ifdef _UNIX
	#include <typeinfo>
#else
	#include <typeinfo.h>
#endif

namespace _2Real
{

	class TypeCategory;
	class Type;

	class TypeDescriptor
	{

	public:

		TypeDescriptor( std::type_info const& info, Type const& type, std::string const& name, TypeDescriptor * nestedType );
		TypeDescriptor( std::type_info const& info, Type const& type, std::string const& name, TypeCategory const& category );
		~TypeDescriptor();

		std::type_info const&			getTypeInfo() const;
		const std::string				getLongTypename() const;
		std::string const&				getTypename() const;
		TypeCategory const&				getTypeCategory() const;
		Type const&						getType() const;
		TypeDescriptor const* const		getNestedType() const;

		bool operator!=( TypeDescriptor const& other ) const;
		bool operator==( TypeDescriptor const& other ) const;

	private:

		TypeDescriptor					*m_NestedType;
		Type							m_Type;
		TypeCategory					m_TypeCategory;
		std::type_info					const& m_TypeInfo;
		std::string						m_HumanReadableName;

	};
}
