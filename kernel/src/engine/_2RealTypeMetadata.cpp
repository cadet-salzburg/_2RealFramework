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
		virtual std::shared_ptr< const TypeConverter > getConverterAtoB() const = 0;
		virtual std::shared_ptr< const TypeConverter > getConverterBtoA() const = 0;
		//virtual bool matches( Fields const&, Fields const& ) = 0;
	};

	class TypeMatchIdentity : public TypeMatch
	{
	public:
		TypeMatchIdentity( TypeMetadata const& a, TypeMetadata const& b );
		std::shared_ptr< const TypeConverter >  getConverterAtoB() const;
		std::shared_ptr< const TypeConverter >  getConverterBtoA() const;
		bool computeMatch( Fields const&, Fields const& );
		bool canConvert() const;
	private:
		Fields		mFa;
		Fields		mFb;
		bool		mIsIdenticalMatch;
	};

	class TypeMatchPerfect : public TypeMatch
	{
	public:
		TypeMatchPerfect( TypeMetadata const& a, TypeMetadata const& b );
		std::shared_ptr< const TypeConverter >  getConverterAtoB() const;
		std::shared_ptr< const TypeConverter >  getConverterBtoA() const;
		bool computeMatch( Fields const&, Fields const&, std::shared_ptr< TypeConverter >, std::shared_ptr< TypeConverter > );
		bool canConvert() const;
	private:
		Fields			mFa;
		Fields			mFb;
		bool			mIsPerfectMatch;
		std::shared_ptr< TypeConverter > mAB;
		std::shared_ptr< TypeConverter > mBA;
	};

	TypeMatchIdentity::TypeMatchIdentity( TypeMetadata const& ta, TypeMetadata const& tb ) : mIsIdenticalMatch( false )
	{
		ta.getFields( mFa );
		tb.getFields( mFb );
		mIsIdenticalMatch = computeMatch( mFa, mFb );
	}

	bool TypeMatchIdentity::canConvert() const
	{
		return mIsIdenticalMatch;
	}

	bool TypeMatchIdentity::computeMatch( Fields const& a, Fields const& b )
	{
		bool match;
		if ( a.size() == b.size() )
		{
			match = true;
			for ( Fields::const_iterator ita = a.begin(), itb = b.begin(); ita != a.end(), itb !=b.end(); ++ita, ++itb )
			{
				FieldRef a = *ita;
				FieldRef b = *itb;

				match &= ( a->getTypename() == b->getTypename() );

				// if no match is possible, stop
				if ( !match ) break;
			}
		}
		else match = false;

		return match;
	}

	std::shared_ptr< const TypeConverter > TypeMatchIdentity::getConverterAtoB() const
	{
		return std::shared_ptr< const TypeConverter >();
	}

	std::shared_ptr< const TypeConverter > TypeMatchIdentity::getConverterBtoA() const
	{
		return std::shared_ptr< const TypeConverter >();
	}

	TypeMatchPerfect::TypeMatchPerfect( TypeMetadata const& ta, TypeMetadata const& tb ) : mIsPerfectMatch( false )
	{
		ta.getFields( mFa );
		tb.getFields( mFb );
		mAB.reset( new TypeConverter ); mAB->mTypeId = tb.getTypeId();
		mBA.reset( new TypeConverter ); mBA->mTypeId = ta.getTypeId();
		mIsPerfectMatch = computeMatch( mFa, mFb, mAB, mBA );
	}

	bool TypeMatchPerfect::canConvert() const
	{
		return mIsPerfectMatch;
	}

	bool TypeMatchPerfect::computeMatch( Fields const& a, Fields const& b, std::shared_ptr< TypeConverter > aToB, std::shared_ptr< TypeConverter > bToA )
	{
		bool match;
		if ( a.size() == b.size() )
		{
			match = true;
			for ( Fields::const_iterator ita = a.begin(), itb = b.begin(); ita != a.end(), itb !=b.end(); ++ita, ++itb )
			{
				FieldRef fa = *ita;
				FieldRef fb = *itb;
				if ( fa->getTypename().first == "basic type" && fb->getTypename().first == "basic type" )
				{
					// reached a simple field -> just rename
					match &= ( fa->getTypename().second == fb->getTypename().second );
					aToB->mLookupTable[ fa->getName() ] = TypeConverter::Conversion( fb->getName(), nullptr );
					bToA->mLookupTable[ fb->getName() ] = TypeConverter::Conversion( fa->getName(), nullptr );
				}
				else
				{
					// reached a complex field -> need new type converter
					std::shared_ptr< TypeConverter > ab( new TypeConverter );
					std::shared_ptr< TypeConverter > ba( new TypeConverter );
					match &= computeMatch( fa->getSubFields(), fb->getSubFields(), ab, ba );
					ab->mTypeId = fb->getTypename();
					ba->mTypeId = fa->getTypename();
					aToB->mLookupTable[ fa->getName() ] = TypeConverter::Conversion( fb->getName(), ab );
					bToA->mLookupTable[ fb->getName() ] = TypeConverter::Conversion( fa->getName(), ba );
				}

				// if no match is possible, stop
				if ( !match ) break;
			}
		}
		else match = false;

		return match;
	}

	std::shared_ptr< const TypeConverter > TypeMatchPerfect::getConverterAtoB() const
	{
		return mAB;
	}

	std::shared_ptr< const TypeConverter > TypeMatchPerfect::getConverterBtoA() const
	{
		return mBA;
	}

	TypeMetadata::TypeMatchSetting::TypeMatchSetting( const Type t ) : mCode( t )
	{
	}

	TypeMetadata::TypeMatchSetting::Type TypeMetadata::TypeMatchSetting::sPriorities[ 2 ] = { IDENTICAL_MATCH, PERFECT_TYPE_MATCH };

	TypeMatch const* TypeMetadata::TypeMatchSetting::createMatch( TypeMetadata const& a, TypeMetadata const& b ) const
	{
		switch( mCode )
		{
		case IDENTICAL_MATCH:
			return new TypeMatchIdentity( a, b );
		case PERFECT_TYPE_MATCH:
			return new TypeMatchPerfect( a, b );
		default:
			return nullptr;
		}
	}

	TypeMetadata::TypeMetadata( TypeId const& id, TypeRegistry const* reg ) : mTypeId( id ), mRegistry( reg ) {}

	TypeMetadata::TypeId const& TypeMetadata::getTypeId() const
	{
		return mTypeId;
	}

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

	bool TypeMetadata::matches( TypeMetadata const& other, TypeMatchSetting const& desiredMatch, std::shared_ptr< const TypeConverter > &cvAB, std::shared_ptr< const TypeConverter > &cvBA ) const
	{
		for ( unsigned int i=0; i<=desiredMatch.getCode(); ++i )
		{
			TypeMatchSetting currentSetting = TypeMatchSetting::sPriorities[ i ];
			TypeMatch const* m = currentSetting.createMatch( *this, other );

			if ( m->canConvert() )
			{
				cvAB = m->getConverterAtoB();
				cvBA = m->getConverterBtoA();
				delete m;
				return true;
			}
			else delete m;
		}
		return false;
	}
}