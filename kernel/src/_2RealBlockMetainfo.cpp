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

#include "_2RealBlockMetainfo.h"
#include "_2RealBlockData.h"
#include "_2RealTypetable.h"

namespace _2Real
{

	BlockMetainfo::BlockMetainfo( BlockData &data, Typetable const& typetable ) :
		m_Impl( data ),
		m_Typetable( typetable )
	{
	}

	void BlockMetainfo::setDescription( std::string const& description )
	{
		m_Impl.setDescription( description );
	}

	void BlockMetainfo::addSetupParameterInternal( std::string const& paramName, EngineData const& defaultValue )
	{
		const std::string type = defaultValue.getTypename();
		const std::string keyword = m_Typetable.lookupKey( type );
		ParameterData *data = new ParameterData( paramName, type, keyword, defaultValue );
		m_Impl.addParameter( *data );
	}

	void BlockMetainfo::addInletInternal( std::string const& inletName, EngineData const& defaultValue )
	{
		const std::string type = defaultValue.getTypename();
		const std::string keyword = m_Typetable.lookupKey( type );
		ParameterData *data = new ParameterData( inletName, type, keyword, defaultValue );
		m_Impl.addInlet( *data );
	}

	void BlockMetainfo::addOutletInternal( std::string const& outletName, std::string const& typeName )
	{
		const std::string keyword = m_Typetable.lookupKey( typeName );
		EngineData empty = m_Typetable.getInitialValueFromKey( keyword );
		ParameterData *data = new ParameterData( outletName, typeName, keyword, empty );
		m_Impl.addOutlet( *data );
	}

}