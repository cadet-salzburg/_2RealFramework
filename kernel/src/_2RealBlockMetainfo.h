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

#include "_2RealEngineData.h"

#include <typeinfo.h>
#include <string>

namespace _2Real
{

	class Typetable;
	class BlockData;

	class BlockMetainfo
	{

	public:

		BlockMetainfo( BlockData &data, Typetable const& typetable );

		template< typename Datatype >
		void addParameter( std::string const& paramName, Datatype defaultValue )
		{
			EngineData data( defaultValue );
			addParameterInternal( setupName, data );
		}

		template< typename Datatype >
		void addInlet( std::string const& inletName, Datatype defaultValue )
		{
			EngineData data( defaultValue );
			addInletInternal( inletName, data );
		}

		template< typename Datatype >
		void addOutlet( std::string const& outletName )
		{
			addOutletInternal( outletName, typeid( Datatype ).name() );
		}

		void setDescription( std::string const& description );

	private:

		void	addSetupParameterInternal( std::string const& paramName, EngineData const& defaultValue );
		void	addInletInternal( std::string const& inletName, EngineData const& defaultValue );
		void	addOutletInternal( std::string const& outletName, std::string const& typeName );

		Typetable		const& m_Typetable;
		BlockData		&m_Impl;

	};

}