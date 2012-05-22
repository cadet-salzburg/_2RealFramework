
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

#include "_2RealBundleInternal.h"
#include "_2RealBundleData.h"
#include "_2RealBlockData.h"
#include "_2RealBlock.h"

#include <iostream>
#include <sstream>

using std::string;
using std::ostringstream;
using std::make_pair;

namespace _2Real
{

	BundleInternal::BundleInternal( string const& name, BundleData const& data ) :
		Entity( name ),
		m_BlockInstances(),
		m_BundleData( data )
	{
	}

	BundleInternal::~BundleInternal()
	{
		for ( BlockMap::iterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); /**/ )
		{
			delete it->second;
			it = m_BlockInstances.erase( it );
		}
	}

	BundleInternal::BundleInternal( BundleInternal const& src ) :
		Entity( src.m_Name ),
		m_BundleData( src.m_BundleData )
	{
	}

	BundleInternal& BundleInternal::operator=( BundleInternal const& src )
	{
		return *this;
	}

	const string BundleInternal::getBundleInfoString() const
	{
		ostringstream info;
		info << m_BundleData;
		return info.str();
	}

	const string BundleInternal::getBlockInfoString( string const& blockName ) const
	{
		ostringstream info;
		info << m_BundleData.getBlockData( blockName );
		return info.str();
	}

	BundleData const& BundleInternal::getBundleData() const
	{
		return m_BundleData;
	}

	BlockData const& BundleInternal::getBlockData( string const& blockName ) const
	{
		return m_BundleData.getBlockData( blockName );
	}

	void BundleInternal::setBundleContext( ServiceBlock &block )
	{
		m_BundleContext = &block;
	}

	ServiceBlock & BundleInternal::getBundleContext()
	{
		return *m_BundleContext;
	}

	unsigned int BundleInternal::getBlockInstanceCount( string const& blockName ) const
	{
		unsigned int counter = 0;
		std::pair< BlockMap::const_iterator, BlockMap::const_iterator > range = m_BlockInstances.equal_range( blockName );
		for ( BlockMap::const_iterator it = range.first; it != range.second; ++it )
		{
			++counter;
		}

		return counter;
	}

	void BundleInternal::addBlockInstance( Block &block, string const& blockName )
	{
		m_BlockInstances.insert( make_pair( blockName, &block ) );
	}

}