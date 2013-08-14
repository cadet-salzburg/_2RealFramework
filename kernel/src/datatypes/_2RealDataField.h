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
#include "helpers/_2RealConstants.h"
#include "helpers/_2RealIdentifiable.h"

namespace _2Real
{
	class DataField;

	typedef std::shared_ptr< const DataField >	DataFieldRef;
	typedef std::vector< const DataFieldRef >	DataFields;

	class DataField
	{

	public:

		std::string const&			getName() const;
		std::string const&			getExportingBundle() const;
		std::string const&			getTypename() const;
		bool						isBasicType() const;
		bool						isFrameworkType() const;
		DataFields					getSubFields() const;

		static DataFieldRef			createSimpleField( std::string const&, std::string const& );
		static DataFieldRef			createComplexField( std::string const&, std::string const&, std::string const&, DataFields const& );

	private:

		DataField( std::string const&, std::string const&, std::string const&, DataFields const& );

		std::string					mFieldName;
		std::string					mExportingBundle;
		std::string					mTypeName;
		DataFields					mSubFields;
	};
}
