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

#include "_2RealEngineData.h"

namespace _2Real
{

	std::ostream& operator<<( std::ostream& out, EngineData const& data )
	{
		data.writeTo( out );
		return out;
	}

	std::istream& operator>>( std::istream& in, EngineData &data )
	{
		data.readFrom( in );
		return in;
	}

	EngineData::EngineData() :
		m_Content()
	{
		AbstractDataHolder *h = nullptr;
		m_Content.reset( h );
	}

	EngineData::EngineData( EngineData const& src ) :
		m_Content( src.m_Content )
	{
	}

	EngineData& EngineData::operator=( EngineData const& src )
	{
		if (this == &src)
		{
			return *this;
		}

		m_Content = src.m_Content;

		return *this;
	}

	bool EngineData::isEmpty() const
	{
		return m_Content.get() == nullptr;
	}

	void EngineData::writeTo(std::ostream &out) const
	{
		if ( !isEmpty() )
		{
			m_Content->writeTo(out);
		}
	}

	void EngineData::readFrom(std::istream &in)
	{
		if ( !isEmpty() )
		{
			m_Content->readFrom(in);
		}
	}

	const std::string EngineData::getTypename() const
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

	void EngineData::cloneFrom( EngineData const& src )
	{

		m_Content.reset( src.m_Content->clone() );
	}

	void EngineData::createNew( EngineData const& src )
	{
		m_Content.reset( src.m_Content->create() );
	}

}