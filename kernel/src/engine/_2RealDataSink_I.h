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

namespace _2Real
{
	class DataSource_I;

	class DataSink_I
	{

	public:

		DataSink_I() = default;
		virtual ~DataSink_I() = default;

		DataSink_I( DataSink_I const& other ) = delete;
		DataSink_I( DataSink_I && other ) = delete;
		DataSink_I& operator=( DataSink_I const& other ) = delete;
		DataSink_I& operator=( DataSink_I && other ) = delete;

		virtual void linkTo( std::shared_ptr< DataSource_I > ) = 0;
		virtual void receiveData( std::shared_ptr< const DataItem > ) = 0;
		virtual boost::signals2::connection registerToRemoved( boost::signals2::signal< void( std::shared_ptr< const DataSink_I > ) >::slot_type ) const = 0;

	};
}