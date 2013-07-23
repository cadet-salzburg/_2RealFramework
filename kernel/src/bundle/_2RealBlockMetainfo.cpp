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

#include "bundle/_2RealBlockMetainfo.h"
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealParameterMetadata.h"
#include "helpers/_2RealStringHelpers.h"
#include "bundle/_2RealTypeMetainfo.h"

namespace _2Real
{
	namespace bundle
	{
		BlockMetainfo::BlockMetainfo( BlockMetadata &data ) :
			m_Impl( data )
		{
		}

		void BlockMetainfo::setDescription( std::string const& description )
		{
			m_Impl.setDescription( description );
		}

		void BlockMetainfo::setCategory( std::string const& category )
		{
			m_Impl.setDescription( category );
		}

		void BlockMetainfo::setThreadingPolicy( ThreadingPolicy const& policy )
		{
			m_Impl.setThreadingPolicy( policy );
		}

		void BlockMetainfo::addCustomTypeInlet( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, InletPolicy const& defaultPolicy )
		{
			privateAddInlet( name, type, init, nullptr, defaultPolicy );
		}

		void BlockMetainfo::privateAddInlet( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, TypeMetadata const* meta, InletPolicy const& defaultPolicy )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			// copy for safety reasons
			std::shared_ptr< const CustomType > copied;
			if ( init.get() )
				copied.reset( new CustomType( *( init.get() ) ) );

			InletMetadata *data = new InletMetadata( trimmed, type, copied, meta, defaultPolicy, false );
			m_Impl.addInlet( data );
		}

		void BlockMetainfo::addCustomTypeParameter( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init )
		{
			privateAddParameter( name, type, init, nullptr );
		}

		void BlockMetainfo::privateAddParameter( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, TypeMetadata const* meta )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			// copy for safety reasons
			std::shared_ptr< const CustomType > copied;
			if ( init.get() )
				copied.reset( new CustomType( *( init.get() ) ) );

			ParameterMetadata *data = new ParameterMetadata( trimmed, type, copied, meta );
			m_Impl.addParameter( data );
		}

		void BlockMetainfo::addCustomTypeOutlet( std::string const& name, std::string const& type )
		{
			privateAddOutlet( name, type, nullptr );
		}

		void BlockMetainfo::privateAddOutlet( std::string const& name, std::string const& type, TypeMetadata const* meta )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			OutletMetadata *data = new OutletMetadata( trimmed, type, meta );
			m_Impl.addOutlet( data );
		}
	}
}