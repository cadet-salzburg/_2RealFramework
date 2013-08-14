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

#include "datatypes/_2RealDataField.h"

namespace _2Real
{
	DataField::DataField( std::string const& name, std::string const& bundle, std::string const& typeName, DataFields const& subfields ) :
		mFieldName( name ),
		mExportingBundle( bundle ),
		mTypeName( typeName ),
		mSubFields( subfields )
	{
	}

	std::string const& DataField::getName() const
	{
		return mFieldName;
	}

	std::string const& DataField::getTypename() const
	{
		return mTypeName;
	}

	std::string const& DataField::getExportingBundle() const
	{
		return mExportingBundle;
	}

	DataFields DataField::getSubFields() const
	{
		return mSubFields;
	}

	bool DataField::isBasicType() const
	{
		return ( mExportingBundle == Constants::BasicTypename );
	}

	bool DataField::isFrameworkType() const
	{
		return ( mExportingBundle == Constants::FrameworkTypename );
	}

	DataFieldRef DataField::createSimpleField( std::string const& name, std::string const& type )
	{
		return DataFieldRef( new DataField( name, Constants::BasicTypename, name, DataFields() ) );
	}

	DataFieldRef DataField::createComplexField( std::string const& name, std::string const& bundle, std::string const& typeName, DataFields const& subfields )
	{
		return DataFieldRef( new DataField( name, bundle, typeName, subfields ) );
	}
}
