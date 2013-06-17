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
#include "helpers/_2RealAnyHolder.h"
#include "datatypes/_2RealFieldDescriptor.h"

#include <map>
#include <list>

namespace _2Real
{
	class TypeMetadata
	{

	private:

		typedef std::map< std::string, FieldDescriptor * >		Fields;

	public:

		TypeMetadata() : mFields() {}

		~TypeMetadata()
		{
			for ( Fields::iterator fieldIter = mFields.begin(); fieldIter != mFields.end(); ++fieldIter )
				delete fieldIter->second;
		}

		void addField( std::string const& name, FieldDescriptor &desc )
		{
			// do a check here??
			mFields[ name ] = &desc;
		}

		unsigned int getNumFields() const
		{
			return mFields.size();
		}

		typedef std::vector< std::string > FieldDesc;

		void getFields( FieldDesc &f ) const
		{
			f.clear(); f.reserve( mFields.size() );
			for ( Fields::const_iterator it = mFields.begin(); it != mFields.end(); ++it )
				f.push_back( it->second->getTypename() );
		}

	private:

		friend class CustomType;

		typedef std::map< std::string, FieldDescriptor * >		Fields;
		Fields													mFields;

	};

	//template< typename TType >
	//struct TypeMetadataFactory;

	//template< >
	//struct TypeMetadataFactory< int >
	//{
	//	// default value, range.... etc
	//	static TypeMetadata *createTypeMetadtata()
	//	{
	//		TypeMetadata *meta = new TypeMetadata();
	//		meta->addField( "default", 0 );
	//		return meta;
	//	}
	//};

	//template< >
	//struct TypeMetadataFactory< NullType >
	//{
	//	static TypeMetadata *createTypeMetadtata()
	//	{
	//		TypeMetadata *meta = new TypeMetadata();
	//		// not adding any fields
	//		return meta;
	//	}
	//};
}