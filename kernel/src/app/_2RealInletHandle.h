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

#include "helpers/_2RealStdIncludes.h"
#include "app/_2RealAbstractInletHandle.h"
#include "engine/_2RealData.h"

namespace _2Real
{
	class Inlet;
	class DataSink;
	class DataSource;

	namespace app
	{
		class LinkHandle;
		class TypeMetainfo;

		class InletHandle : public AbstractInletHandle
		{

		public:

			explicit InletHandle( std::shared_ptr< Inlet > );
	
			using AbstractInletHandle::isValid;
			using AbstractInletHandle::isMultiInlet;
			using AbstractInletHandle::getBlock;

			// TODO: these should definitely throw an exception in case of a datatype mismatch
			//void setValue( DataItem && value );
			//void setValue( std::shared_ptr< DataItem > value );
			void setValue( DataItem value );

			LinkHandle linkTo( std::shared_ptr< DataSource > );

			operator std::shared_ptr< DataSink > ();

		private:

			friend class MultiInletHandle;

			std::weak_ptr< Inlet >		mImpl;

		};
	}
}
