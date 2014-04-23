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

#include "engine/_2RealLink.h"
#include "engine/_2RealInletImpl.h"
#include "engine/_2RealOutletImpl.h"
#include "engine/_2RealLinkCollection.h"

namespace _2Real
{
	class LinkByAddress : public std::unary_function< bool, std::pair< boost::signals2::connection, std::shared_ptr< Link > > >
	{
	public:
		explicit LinkByAddress( std::shared_ptr< const Link > link ) : mBaseline( link ) { assert( link.get() ); }
		bool operator()( std::pair< boost::signals2::connection, std::shared_ptr< Link > > const& val ) const
		{
			assert( val.second.get() );
			return mBaseline.get() == val.second.get();
		}
	private:
		std::shared_ptr< const Link > mBaseline;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	void LinkCollection::clear()
	{
		for ( auto &it : mLinks )
			it.first.disconnect();

		mLinks.clear();
	}

	std::shared_ptr< Link > LinkCollection::createLink( std::shared_ptr< InletImpl > inlet, std::shared_ptr< OutletImpl > outlet )
	{
		auto sink = std::static_pointer_cast< DataSink_I >( inlet );
		auto source = std::static_pointer_cast< DataSource_I >( outlet );
		std::shared_ptr< Link > link = Link::createLink( sink, source );

		auto connection = link->registerToDestroyed( std::bind( &LinkCollection::linkDestroyed, this, std::placeholders::_1 ) );
		mLinks.push_back( std::make_pair( connection, link ) );
		return link;
	}

	void LinkCollection::linkDestroyed( std::shared_ptr< const Link > link )
	{
		auto it = std::find_if( mLinks.begin(), mLinks.end(), LinkByAddress( link ) );
		if ( it != mLinks.end() )
		{
			it->first.disconnect();
			mLinks.erase( it );
		}
	}
}
