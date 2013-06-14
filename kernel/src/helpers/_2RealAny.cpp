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

#include "helpers/_2RealAny.h"
#include "datatypes/_2RealTypes.h"

namespace _2Real
{
	//Any::Any() :
	//	m_Content( new AnyHolder< NullType >() ),
	//	m_TypeDescriptor( createTypeDescriptor< NullType >() )
	//{
	//}

	//Any::Any( Any const& src ) :
	//	m_Content( src.m_Content ),
	//	m_TypeDescriptor( src.m_TypeDescriptor )
	//{
	//}

	//Any& Any::operator=( Any const& src )
	//{
	//	if (this == &src)
	//	{
	//		return *this;
	//	}

	//	m_Content = src.m_Content;
	//	m_TypeDescriptor = src.m_TypeDescriptor;

	//	return *this;
	//}

	//bool Any::isNull() const
	//{
	//	return isDatatype< NullType >();
	//}

	//bool Any::isEqualTo( Any const& any ) const
	//{
	//	return m_Content->isEqualTo( *any.m_Content.get() );
	//}

	//bool Any::isLessThan( Any const& any ) const
	//{
	//	return m_Content->isLessThan( *any.m_Content.get() );
	//}

	//void Any::writeTo(std::ostream &out) const
	//{
	//	m_Content->writeTo(out);
	//}

	//void Any::readFrom(std::istream &in)
	//{
	//	m_Content->readFrom(in);
	//}

	//Type const& Any::getType() const
	//{
	//	return m_TypeDescriptor->m_Type;
	//}

	//TypeCategory const& Any::getTypeCategory() const
	//{
	//	return m_TypeDescriptor->m_TypeCategory;
	//}

	//void Any::cloneFrom( Any const& src )
	//{
	//	m_Content.reset( src.m_Content->clone() );
	//	m_TypeDescriptor = src.m_TypeDescriptor;
	//}

	//void Any::createNew( Any const& src )
	//{
	//	m_Content.reset( src.m_Content->create() );
	//	m_TypeDescriptor = src.m_TypeDescriptor;
	//}

}