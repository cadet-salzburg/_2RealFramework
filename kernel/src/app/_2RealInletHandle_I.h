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

/*
*	inlet handle interface: because BlockHandle::getBlockIo will return all inlets in one std::vector,
*	regardless of whether or not they are multiinlets or not
*/

namespace _2Real
{
	namespace app
	{
		class BlockHandle;

		class InletHandle_I
		{

		public:

			virtual ~InletHandle_I() = default;	

			/*
			*	@return: true if underlying object is valid ( all io slots are destroyed when the parent block is destroyed )
			*/
			virtual bool			isValid() const = 0;
	
			/*
			*	@return: true if inlet is a multiinlet, false otherwise
			*/
			virtual bool			isMultiInlet() const = 0;

			/*
			*	@return: handle to owning block
			*/
			virtual BlockHandle		getBlock() = 0;

		};
	}
}
