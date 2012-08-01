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

#include "helpers/_2RealAny.h"
#include "datatypes/_2RealTypes.h"

#ifdef _UNIX
	#include <typeinfo>
#else
	#include <typeinfo.h>
#endif
#include <string>

namespace _2Real
{
	class BlockMetadata;

	namespace bundle
	{
		class ContextBlockMetainfo
		{

		public:

			ContextBlockMetainfo( BlockMetadata &data );

			void setDescription( std::string const& description );

			template< typename TData >
			void addOutlet( std::string const& name )
			{
				TypeDescriptor *d = createTypeDescriptor< TData >();
				TData data;
				Any init( data );
				addOutletInternal( name, *d, init );
			}

		private:

			void		addOutletInternal( std::string const& name, TypeDescriptor &descriptor, Any const& init );

			BlockMetadata	&m_Impl;

		};
	}
}
