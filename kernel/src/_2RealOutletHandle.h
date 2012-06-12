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

#include "_2RealParameterHandle.h"
#include "_2RealEngineData.h"

namespace _2Real
{

	class Outlet;

	class OutletHandle : public ParameterHandle
	{

	public:

		OutletHandle();
		OutletHandle( Outlet &slot );
		OutletHandle( OutletHandle const& src );
		OutletHandle& operator=( OutletHandle const& src );

		/**
		*	this returns a reference to the outlets' current data item
		*	a block is expected to write the appropriate values into it
		*	once per update cycle, otherwise its value is undefined
		*	if a block wishes to not write anything in an update cycle,
		*	it must explicitly call discard() !
		*/
		template< typename Datatype >
		Datatype & data()
		{
			if (!m_Output)
			{
				std::ostringstream msg;
				msg << "output handle was not initialized by framework";
				throw UninitializedHandleException(msg.str());
			}

			std::shared_ptr< Datatype > ptr = extractFrom< Datatype >(data());
			return *ptr.get();
		}

		/**
		*	call this function if you wish to write another data item
		*	in a single update cycle; note that if you call this, you must
		*	also assign a value to it - otherwise its value will be undefined
		*
		*	note: not sure how useful this function actually is, blocks
		*	that trigger on 'new data' will almost certainly be busy processing
		*	the first data item sent to them & ignore the following ones -
		*	but then, it might make sense for some other configurations,
		*	e.g. if this belongs to a block with very slow update rate 
		*	& is linked to a block updating on 'data available'
		*
		*	also, i might rename this at some point
		*/
		void otherDataItem();

		/**
		*	discards the outlets current data item,
		*	meaning that this item will not be sent to linked inlets
		*	or registered callbacks at the end of the current update cycle
		*	if otherDataItem() was called, the function discards the data
		*	that was created last
		*/
		void discard();

	private:

		EngineData			data();
		Outlet				*m_Output;

	};

}