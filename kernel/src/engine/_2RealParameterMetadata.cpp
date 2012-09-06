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

#include "engine/_2RealParameterMetadata.h"
#include "helpers/_2RealTypeDescriptor.h"

using std::string;

namespace _2Real
{

	ParameterMetadata::ParameterMetadata( string const& name, TypeDescriptor &descriptor, Any const& initialValue ) :
		m_Name( name ),
		m_TypeDescriptor( &descriptor ),
		m_InitialValue( initialValue )
	{
	}

	ParameterMetadata::~ParameterMetadata()
	{
		delete m_TypeDescriptor;
	}

	void ParameterMetadata::enableOptions( AnyOptionSet const& options )
	{
		m_Options = options;
	}

	bool ParameterMetadata::hasOptions() const
	{
		return !m_Options.isEmpty();
	}

	AnyOptionSet const& ParameterMetadata::getOptions() const
	{
		return m_Options;
	}

	string const& ParameterMetadata::getName() const
	{
		return m_Name;
	}

	TypeDescriptor const& ParameterMetadata::getTypeDescriptor() const
	{
		return *m_TypeDescriptor;
	}

	string const& ParameterMetadata::getTypename() const
	{
		return m_TypeDescriptor->m_TypeName;
	}

	const string ParameterMetadata::getLongTypename() const
	{
		return m_TypeDescriptor->m_LongTypename;
	}

	Any const& ParameterMetadata::getInitialValue() const
	{
		return m_InitialValue;
	}

	void ParameterMetadata::performParameterNameCheck( std::string const& name )
	{
	}

}