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

#include "_2RealAny.h"

namespace _2Real
{

	std::ostream& operator<<( std::ostream& out, Any const& data )
	{
		data.writeTo( out );
		return out;
	}

	std::istream& operator>>( std::istream& in, Any &data )
	{
		data.readFrom( in );
		return in;
	}

	Any::Any() :
		m_Content()
	{
		AbstractAnyHolder *h = nullptr;
		m_Content.reset( h );
	}

	Any::Any( Any const& src ) :
		m_Content( src.m_Content )
	{
	}

	Any& Any::operator=( Any const& src )
	{
		if (this == &src)
		{
			return *this;
		}

		m_Content = src.m_Content;

		return *this;
	}

	bool Any::isEmpty() const
	{
		return m_Content.get() == nullptr;
	}

	void Any::writeTo(std::ostream &out) const
	{
		if ( !isEmpty() )
		{
			m_Content->writeTo(out);
		}
	}

	void Any::readFrom(std::istream &in)
	{
		if ( !isEmpty() )
		{
			m_Content->readFrom(in);
		}
	}

	const std::string Any::getTypename() const
	{
		if ( isEmpty() )
		{
			return typeid( void ).name();
		}
		else
		{
			return m_Content->getTypename();
		}
	}

	void Any::cloneFrom( Any const& src )
	{

		m_Content.reset( src.m_Content->clone() );
	}

	void Any::createNew( Any const& src )
	{
		m_Content.reset( src.m_Content->create() );
	}

}