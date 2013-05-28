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

#include "datatypes/_2RealCustomData.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "engine/_2RealTypeMetadata.h"

namespace _2Real
{
	CustomType::CustomType( bundle::TypeMetainfo const& meta )
	{
		TypeMetadata const& metadata = meta.mImpl;
		for ( TypeMetadata::Fields::const_iterator it = metadata.mFields.begin(); it != metadata.mFields.end(); ++it )
		{
			// create an any of appropriate type
			Any init = ( it->second )->createAny();
			// add it to map ( or whatever structure is used )
			this->initField( it->first, init );
		}
	}

	void CustomType::initField( std::string const& name, Any &any )
	{
#ifdef _DEBUG
		if ( any.isNull() )
		{
			// TODO ASSERT
		}

		if ( mDataFields.find( name ) != mDataFields.end() )
		{
			// TODO ASSERT
		}
#else
		// ONLY here, in the init field method, assignment operator may be used
		// for all later operations, set must be used instead ( involves a typecheck! )
		mDataFields[ name ] = any;
#endif
	}

	CustomType::DataFields::iterator CustomType::iter( std::string const& name )
	{
		DataFields::iterator dataIter = mDataFields.find( name );
		if ( dataIter == mDataFields.end() )
		{
			// TODO exception
		}
			
		return dataIter;
	}

	CustomType::DataFields::const_iterator CustomType::constIter( std::string const& name ) const
	{
		DataFields::const_iterator dataIter = mDataFields.find( name );
		if ( dataIter == mDataFields.end() )
		{
			// TODO exception
		}
			
		return dataIter;
	}

	void CustomType::setValueInternal( std::string const& field, Any &val/*, Range &range*/)
	{
		// may throw
		DataFields::iterator it = iter( field );
		// may also throw
		( it->second ).set( val/*, range*/ );
	}
}
