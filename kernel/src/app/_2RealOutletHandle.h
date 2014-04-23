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
#include "common/_2RealSignals.h"
#include "common/_2RealData.h"

namespace _2Real
{
	class OutletImpl;

	namespace app
	{
		class BlockHandle;

		class OutletHandle
		{

		public:

			/*
			*	created by block handle via BlockHandle::getOutlet
			*/
			explicit OutletHandle( std::shared_ptr< OutletImpl > );

			/*
			*	@return: true if underlying object is valid ( all io slots are destroyed when the parent block is destroyed )
			*/
			bool				isValid() const;

			/*
			*	@return: handle to parent block
			*/
			BlockHandle			getBlock();

			/*
			*	allows you to register a function that is called whenever the outlet value changes
			*	( i.e., once after each parent update )
			*	the returned DataItem is constant and must absolutely not be modified
			*	( because the value may be read by other blocks any time )
			*/
			Connection registerToNewData( boost::signals2::signal< void( std::shared_ptr< const DataItem > ) >::slot_type );

		private:

			friend class Engine;

			std::weak_ptr< OutletImpl >		mImpl;

		};
	}
}
