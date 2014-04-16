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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealData.h"
#include "common/_2RealEvent_T.h"

namespace _2Real
{
	class DataSource_I
	{

	public:

		DataSource_I() = default;
		virtual ~DataSource_I() = default;

		DataSource_I( DataSource_I const& other ) = delete;
		DataSource_I( DataSource_I && other ) = delete;
		DataSource_I& operator=( DataSource_I const& other ) = delete;
		DataSource_I& operator=( DataSource_I && other ) = delete;

		template< typename TCallable >
		void registerToNewData( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const DataItem > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const DataItem > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const DataItem > >( callable, callback ) );
			mNewDataEvent.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromNewData( TCallable *callable, void ( TCallable::*callback )( std::shared_ptr< const DataItem > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const DataItem > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const DataItem > >( callable, callback ) );
			mNewDataEvent.removeListener( listener );
		}

	protected:

		void notify( std::shared_ptr< const DataItem > data) { mNewDataEvent.notify( data ); }

	private:

		Event_T< std::shared_ptr< const DataItem > >		mNewDataEvent;

	};
}