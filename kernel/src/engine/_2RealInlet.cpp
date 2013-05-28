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

#include "engine/_2RealInlet.h"
#include "bundle/_2RealInletHandle.h"
#include "engine/_2RealAbstractUberBlock.h"

using std::string;

namespace _2Real
{
	AbstractInlet::AbstractInlet( AbstractUberBlock &owningBlock, string const& name ) :
		NonCopyable< AbstractInlet >(),
		Identifiable< AbstractInlet >( owningBlock.getIds(),  name ),
		Handleable< AbstractInlet, bundle::InletHandle >( *this )
	{
	}

	BasicInlet::BasicInlet( AbstractUberBlock &owningBlock, string const& name ) :
		AbstractInlet( owningBlock, name )
		, mLastData()
		, mCurrentData()
	{
	}

	std::shared_ptr< const CustomType > BasicInlet::getCurrentData() const
	{
		return mCurrentData;
	}

	bool BasicInlet::hasChanged() const
	{
		return ( !mCurrentData->isEqualTo( *( mLastData.get() ) ) );
	}

	void BasicInlet::setData( std::shared_ptr< const CustomType > const& data )
	{
		mLastData = mCurrentData;
		mCurrentData = data;
	}

	MultiInlet::MultiInlet( AbstractUberBlock &owningBlock, string const& name ) :
		AbstractInlet( owningBlock, name )
	{
	}

	MultiInlet::~MultiInlet()
	{
	}

	BasicInlet & MultiInlet::operator[]( const unsigned int index )
	{
		try
		{
			return *m_Inlets.at( index );
		}
		catch ( std::out_of_range &e )
		{
			throw _2Real::Exception( e.what() );
		}
	}

	void MultiInlet::addBasicInlet( BasicInlet &inlet )
	{
		m_Inlets.push_back( &inlet );
	}

	void MultiInlet::removeBasicInlet( BasicInlet &inlet )
	{
		for ( BasicInletIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( &inlet == *it )
			{
				m_Inlets.erase( it );
				break;
			}
		}
	}
}