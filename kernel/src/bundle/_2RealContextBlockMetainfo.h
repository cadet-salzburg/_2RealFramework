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

#include <string>

namespace _2Real
{
	class BlockData;
	class Typetable;

	namespace bundle
	{
		class ContextBlockMetainfo
		{

		public:

			ContextBlockMetainfo( BlockData &data, Typetable const& typetable );

			void setDescription( std::string const& description );

			template< typename Datatype >
			void addOutlet( std::string const& outletName )
			{
				addOutletInternal( outletName, typeid( Datatype ).name() );
			}

		private:

			void		addOutletInternal( std::string const& outletName, std::string const& longTypename );

			BlockData	&m_Impl;
			Typetable	const& m_Typetable;

		};
	}
}