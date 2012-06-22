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

#include "_2RealSetupParameter.h"
#include "_2RealParameterData.h"
#include "_2RealException.h"

#include <sstream>

using std::string;
using std::ostringstream;

namespace _2Real
{

	SetupParameter::SetupParameter( string const& name, string const& longTypename, string const& typeName, EngineData const& initialValue ) :
		Parameter( name, longTypename, typeName ),
		m_Value( initialValue )
	{
	}

	void SetupParameter::setParameterValue( EngineData const& value )
	{
		if ( value.getTypename() != Parameter::getLongTypename() )
		{
			ostringstream msg;
			msg << "datatype mismatch: " << Parameter::getLongTypename() << " vs. template parameter type " << value.getTypename();
			throw TypeMismatchException(msg.str());
		}

		m_Value = value;
	}

	EngineData const& SetupParameter::getParameterValue() const
	{
		return m_Value;
	}

}