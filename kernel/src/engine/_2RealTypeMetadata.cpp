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

#include "datatypes/_2RealFieldDescriptor.h"
#include "datatypes/_2RealTypeRegistry.h"
#include "datatypes/_2RealDataField.h"
#include "datatypes/_2RealTypeConverter.h"
#include "_2RealTypeMetadata.h"

namespace _2Real
{
	class TypeMatch
	{
	public:
		virtual ~TypeMatch() {}
		virtual bool canConvert() const = 0;
		virtual TypeConverter const* getConverterAtoB() const = 0;
		virtual TypeConverter const* getConverterBtoA() const = 0;
	};

	class TypeMatchPerfect : public TypeMatch
	{
	public:
		TypeMatchPerfect( TypeMetadata const& a, TypeMetadata const& b );
		bool canConvert() const;
		TypeConverter const* getConverterAtoB() const;
		TypeConverter const* getConverterBtoA() const;
	private:
		Fields		mFa;
		Fields		mFb;
		bool		mIsPerfectMatch;
	};

	TypeMatchPerfect::TypeMatchPerfect( TypeMetadata const& ta, TypeMetadata const& tb ) : mIsPerfectMatch( false )
	{
		ta.getFields( mFa );
		tb.getFields( mFb );

		bool fieldMatch = false;
		if ( mFa.size() == mFb.size() )
		{
			fieldMatch = true;
			for ( Fields::iterator ita = mFa.begin(), itb = mFb.begin(); ita != mFa.end(), itb != mFb.end(); ++ita, ++itb )
			{
				if ( ( *ita )->getTypename() != ( *itb )->getTypename() )
					fieldMatch = false;
			}
		}

		mIsPerfectMatch = fieldMatch;
	}

	bool TypeMatchPerfect::canConvert() const
	{
		return mIsPerfectMatch;
	}

	TypeConverter const* TypeMatchPerfect::getConverterAtoB() const
	{
		return nullptr;
	}

	TypeConverter const* TypeMatchPerfect::getConverterBtoA() const
	{
		return nullptr;
	}

	TypeMetadata::TypeMatchSetting::TypeMatchSetting( const Type t ) : mCode( t )
	{
	}

	TypeMatch const* TypeMetadata::TypeMatchSetting::createMatch( TypeMetadata const& a, TypeMetadata const& b ) const
	{
		switch( mCode )
		{
		case PERFECT_MATCH:
			return new TypeMatchPerfect( a, b );
		default:
			return nullptr;
		}
	}

	TypeMetadata::TypeMetadata( TypeId const& id, TypeRegistry const* reg ) : mTypeId( id ), mRegistry( reg ) {}

	void TypeMetadata::addField( std::string const& name, TypeId const& id, FieldDescriptorRef desc )
	{
		// fields: added with type id instead of just type name

		// make sure field name is unique
		for ( FieldDescriptions::iterator it = mFields.begin(); it != mFields.end(); ++it )
		{
			if ( name == it->first )
			{
				std::ostringstream msg;
				msg << "field: " << name << " already exists in type::" << mTypeId.first << "::" << mTypeId.second << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		// if desc == null, it's a custom type, so get the metadata
		if ( nullptr == desc )
		{
#ifdef _DEBUG
			assert( mRegistry );
#endif
			TypeMetadata const* meta = mRegistry->get( TypeRegistry::sFrameworkTypes, id.second );
			if ( nullptr == meta )
			{
				meta = mRegistry->get( mTypeId.first, id.second );
				if ( nullptr == meta )
				{
					std::stringstream msg;
					msg << "type::" << mTypeId.first << "::" << id.second << " is not known";
					throw NotFoundException( msg.str() );
				}

				_2Real::Fields fields;
				meta->getFields( fields );

				FieldDescriptorRef d( DataField< CustomType >::createFieldDescriptor( name, id, CustomType( meta ), fields ) );
				mFields.push_back( std::make_pair( name, d ) );
			}
			else
			{
				_2Real::Fields fields;
				meta->getFields( fields );

				FieldDescriptorRef d( DataField< CustomType >::createFieldDescriptor( name, TypeId( TypeRegistry::sFrameworkTypes, id.second ), CustomType( meta ), fields ) );
				mFields.push_back( std::make_pair( name, d ) );
			}
		}
		else
			mFields.push_back( std::make_pair( name, desc ) );
	}

	void TypeMetadata::getFields( _2Real::Fields &fields ) const
	{
		fields.clear(); fields.reserve( mFields.size() );
		for ( FieldDescriptions::const_iterator it = mFields.begin(); it != mFields.end(); ++it )
		{
			fields.push_back( it->second->getField() );
		}
	}

	bool TypeMetadata::matches( TypeMetadata const& other, TypeMatchSetting const& desiredMatch, TypeConverter const*& cvAB, TypeConverter const*& cvBA ) const
	{
		TypeMatch const* m = desiredMatch.createMatch( *this, other );

		cvAB = m->getConverterAtoB();
		cvBA = m->getConverterBtoA();

		return m->canConvert();
	}
}