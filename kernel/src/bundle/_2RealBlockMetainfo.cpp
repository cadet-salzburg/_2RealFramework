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
#include "engine/_2RealTypetable.h"

namespace _2Real
{
	namespace bundle
	{
		BlockMetainfo::BlockMetainfo( BlockMetadata &data, Typetable const& typetable ) :
			m_Impl( data ),
			m_Typetable( typetable )
		{
		}

		void BlockMetainfo::setDescription( std::string const& description )
		{
			m_Impl.setDescription( description );
		}

		void BlockMetainfo::addInletInternal( std::string const& inletName, Any const& defaultValue )
		{
			const std::string longTypename = defaultValue.getTypename();
			const std::string typeName = m_Typetable.lookupTypename( longTypename );
			ParameterMetadata data( inletName, longTypename, typeName, defaultValue );
			m_Impl.addInlet( data );
		}

		void BlockMetainfo::addOutletInternal( std::string const& outletName, std::string const& longTypename )
		{
			const std::string typeName = m_Typetable.lookupTypename( longTypename );
			Any const& defaultConstructed = m_Typetable.getInitialValueFromTypename( typeName );

			// the outlet does not get a default value, but the data needs to be allocated anyway
			// so copy the value stored inside the typetable ( constructed with default ctor )
			Any val;
			val.cloneFrom( defaultConstructed );

			ParameterMetadata data( outletName, longTypename, typeName, val );
			m_Impl.addOutlet( data );
		}
	}
}