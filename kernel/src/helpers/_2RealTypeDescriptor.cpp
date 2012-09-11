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

#include "helpers/_2RealTypeDescriptor.h"

namespace _2Real
{
		TypeDescriptor::TypeDescriptor( std::type_info const& info, Type const& type, std::string const& name, TypeDescriptor * nestedType ) :
			m_NestedType( nestedType ),
			m_Type( type ),
			m_TypeCategory( TypeCategory::CONTAINER ),
			m_TypeInfo( info ),
			m_TypeName( std::string( nestedType->m_TypeName ).append(" ").append( name ) ),
			m_LongTypename( info.name() )
		{
		}

		TypeDescriptor::TypeDescriptor( std::type_info const& info, Type const& type, std::string const& name, TypeCategory const& category ) :
			m_NestedType( nullptr ),
			m_Type( type ),
			m_TypeCategory( category ),
			m_TypeInfo( info ),
			m_TypeName( name ),
			m_LongTypename( info.name() )
		{
		}

		TypeDescriptor::~TypeDescriptor()
		{
			delete m_NestedType;
		}

		//std::type_info const& TypeDescriptor::getTypeInfo() const
		//{
		//	return m_TypeInfo;
		//}

		//std::string const& TypeDescriptor::getTypename() const
		//{
		//	return m_HumanReadableName;
		//}

		//const std::string TypeDescriptor::getLongTypename() const
		//{
		//	return m_TypeInfo.name();
		//}

		//TypeCategory const& TypeDescriptor::getTypeCategory() const
		//{
		//	return m_TypeCategory;
		//}

		//Type const& TypeDescriptor::getType() const
		//{
		//	return m_Type;
		//}

		//TypeDescriptor const* const TypeDescriptor::getNestedType() const
		//{
		//	return m_NestedType;
		//}

		bool TypeDescriptor::operator!=( TypeDescriptor const& other ) const
		{
			return !( operator==( other ) );
		}

		bool TypeDescriptor::operator==( TypeDescriptor const& other ) const
		{
			if ( m_Type == other.m_Type )
			{
				if ( m_NestedType == nullptr )
				{
					if ( other.m_NestedType == nullptr )
					{
						return true;
					}
					else return false;
				}
				else
				{
					if ( other.m_NestedType == nullptr )
					{
						return false;
					}
					else return ( *m_NestedType == *other.m_NestedType );
				}
			}
			else return false;
		}
}