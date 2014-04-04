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
#include "engine/_2RealDataSource.h"
#include "engine/_2RealDataSink.h"
#include "helpers/_2RealException.h"

namespace _2Real
{
	Link::Link( std::shared_ptr< DataSource > source, std::shared_ptr< DataSink > sink ) :
		std::enable_shared_from_this< Link >(),
		mSource( source ),
		mSink( sink )
	{
			// TODO: check datatypes of source & sink
			source->registerToNewData( this, &Link::receiveData );
	}

	Link::~Link()
	{
		mSource.lock()->unregisterFromNewData( this, &Link::receiveData );
	}

	bool Link::isValid() const
	{
		return ( mSource.lock() && mSink.lock() );
	}

	void Link::receiveData( std::shared_ptr< const DataItem > data )
	{
		// TODO: auto conversion?

		std::shared_ptr< DataSink > sink = mSink.lock();
		if ( !sink.get() )
		{
			std::ostringstream msg;
			msg << "data sink no longer exists" << std::endl;
			throw _2Real::NotFound( msg.str() );
		}

		sink->receiveData( data );
	}
}
