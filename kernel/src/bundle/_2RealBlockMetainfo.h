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

#include "helpers/_2RealEngineData.h"

#include <typeinfo.h>
#include <string>

namespace _2Real
{
	class Typetable;
	class BlockData;

	namespace bundle
	{
		class BlockMetainfo
		{

		public:

			BlockMetainfo( BlockData &data, Typetable const& typetable );

			void setDescription( std::string const& description );

			template< typename Datatype >
			void addInlet( std::string const& inletName, Datatype defaultValue )
			{
				addInletInternal( inletName, EngineData( defaultValue ) );
			}

			template< typename Datatype >
			void addOutlet( std::string const& outletName )
			{
				addOutletInternal( outletName, typeid( Datatype ).name() );
			}

		private:

			void		addInletInternal( std::string const& inletName, EngineData const& defaultValue );
			void		addOutletInternal( std::string const& outletName, std::string const& longTypename );

			BlockData	&m_Impl;
			Typetable	const& m_Typetable;

		};
	}
}