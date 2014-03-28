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

#include "helpers/_2RealBoost.h"
#include "helpers/_2RealException.h"

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 40

#include <boost/variant.hpp>

#include "engine/_2RealCloneableMetainfo.h"

namespace _2Real
{
	class CustomDataItem;

	/*
		this is our datatype
	*/
	typedef boost::variant< uint8_t,// std::vector< uint8_t >,
							int8_t,// std::vector< int8_t >,
							uint32_t,// std::vector< uint32_t >,
							int32_t,// std::vector< int32_t >,
							uint64_t,// std::vector< uint64_t >,
							int64_t,// std::vector< int64_t >,
							double,// std::vector< double >,
							float,// std::vector< float >,
							std::string,// std::vector< std::string >,
							bool,// std::vector< bool >,
							boost::recursive_wrapper< CustomDataItem >//, std::vector< boost::recursive_wrapper< CustomType > > >
							> DataItem;
	/*
	*	helper
	*/
	typedef boost::variant< uint8_t,// std::vector< uint8_t >,
							int8_t,// std::vector< int8_t >,
							uint32_t,// std::vector< uint32_t >,
							int32_t,// std::vector< int32_t >,
							uint64_t,// std::vector< uint64_t >,
							int64_t,// std::vector< int64_t >,
							double,// std::vector< double >,
							float,// std::vector< float >,
							std::string,// std::vector< std::string >,
							bool// std::vector< bool >,
							> BasicDataItem;



	class DataField
	{

	public:

		DataField( std::string fieldName, DataItem value ) :
			mName( std::move( fieldName ) ),
			mValue( std::move( value ) )
		{ 
			//std::cout << "data field init ctor" << std::endl;
		}

		DataField( DataField const& other ) :
			mName( other.mName ),
			mValue( other.mValue )
		{ 
			//std::cout << "data field copy ctor" << std::endl;
		}

		DataField( DataField && other ) :
			mName( std::move( other.mName ) ),
			mValue( std::move( other.mValue ) )
		{
			//std::cout << "data field move ctor" << std::endl;
		}

		DataField& operator=( DataField const& other )
		{
			if ( this == &other )
				return *this;

			mName = other.mName;
			mValue = other.mValue;

			//std::cout << "data field copy assignment" << std::endl;

			return *this;
		}

		DataField& operator=( DataField && other )
		{
			if ( this == &other )
				return *this;

			mName = std::move( other.mName );
			mValue = std::move( other.mValue );

			//std::cout << "data field move assignment" << std::endl;

			return *this;
		}

		~DataField() = default;

		std::string const& getName() const
		{
			return mName;
		}

		DataItem const& getValue() const
		{
			return mValue;
		}

		DataItem & getValue()
		{
			return mValue;
		}

		void setValue( DataItem value )
		{
			mValue = std::move( value );
		}

	private:

		std::string		mName;
		DataItem		mValue;

	};

	class CustomDataItem
	{

	private:

		std::string								mTypename;
		typedef std::vector< DataField >		DataFields;
		DataFields								mDataFields;

	public:

		CustomDataItem();
		CustomDataItem( CustomDataItem const& other );
		CustomDataItem( CustomDataItem && other );
		CustomDataItem& operator=( CustomDataItem const& other );
		CustomDataItem& operator=( CustomDataItem && other );
		~CustomDataItem() = default;

		std::string const& getName() const;
		void set( std::string const& fieldName, DataItem value );
		void addField( DataField field );

		friend std::ostream& operator<<( std::ostream& out, CustomDataItem const& val );

		template< typename TData >
		TData const& getValue( std::string const& fieldName ) const
		{
			try
			{
				for ( auto &it : mDataFields )
				{
					if ( it.getName() == fieldName )
						return boost::get< TData >( it.getValue() );
				}

				throw _2Real::NotFoundException( fieldName );
			}
			catch ( boost::bad_get const& e )
			{
				throw _2Real::TypeMismatchException( fieldName );
			}
		}

		template< typename TData >
		TData & getValue( std::string const& fieldName )
		{
			try
			{
				for ( auto &it : mDataFields )
				{
					if ( it.getName() == fieldName )
						return boost::get< TData >( it.getValue() );
				}

				throw _2Real::NotFoundException( fieldName );
			}
			catch ( boost::bad_get const& e )
			{
				throw _2Real::TypeMismatchException( fieldName );
			}
		}

	};

	class TypeDescriptor;

	class FieldDescriptor
	{

	public:

		std::string							mName;
		std::shared_ptr< TypeDescriptor >	mDescription;

	};

	class TypeDescriptor
	{

	public:

		virtual ~TypeDescriptor() {}

		virtual bool isBasicType() const = 0;
		//virtual std::shared_ptr< DataItem >	makeData() const = 0;
		//virtual std::string const& getTypeName() const = 0;

	};

	class BasicTypeDescriptor
	{

	public:

		bool isBasicType() const
		{
			return true;
		}

		std::string const& getDatatype() const
		{
			return mTypename;
		}

	private:

		std::string		mTypename;

	};

	class CustomTypeDescriptor// : public CloneableMetainfo< CustomTypeDescriptor >
	{

	private:

		typedef std::vector< std::shared_ptr< FieldDescriptor > >	Fields;
		Fields														mFields;

	public:

		bool isBasicType() const
		{
			return false;
		}

		std::vector< std::shared_ptr< FieldDescriptor > > const& getFields() const
		{
			return mFields;
		}

		void cloneFrom( CustomTypeDescriptor const& other )
		{
			if ( this == &other )
				return;

			mTypename = other.mTypename;
		}

	private:

		std::string		mTypename;

	};


//}
//
//#include "engine/_2RealCustomData.h"
//
//namespace _2Real
//{

	class HumanReadableNameVisitor : public boost::static_visitor< std::string >
	{

	public:

		std::string operator()( const uint8_t val ) const
		{
			( void )( val );
			return "unsigned char";
		}

		std::string operator()( const int8_t val ) const
		{
			( void )( val );
			return "char";
		}

		std::string operator()( const uint32_t val ) const
		{
			( void )( val );
			return "unsigned int";
		}

		std::string operator()( const int32_t val ) const
		{
			( void )( val );
			return "int";
		}

		std::string operator()( const uint64_t val ) const
		{
			( void )( val );
			return "unsigned long";
		}

		std::string operator()( const int64_t val ) const
		{
			( void )( val );
			return "long";
		}

		std::string operator()( double const& val ) const
		{
			( void )( val );
			return "double";
		}

		std::string operator()( const float val ) const
		{
			( void )( val );
			return "float";
		}

		std::string operator()( std::string const& val ) const
		{
			( void )( val );
			return "string";
		}

		std::string operator()( const bool val ) const
		{
			( void )( val );
			return "bool";
		}

		std::string operator()( _2Real::CustomDataItem const& val ) const
		{
			return val.getName();
		}

	};



	//class InitialValueVisitor : public boost::static_visitor< std::string >
	//{

	//public:

	//	std::string operator()( const uint8_t val ) const
	//	{
	//		( void )( val );
	//		return "unsigned char";
	//	}

	//	std::string operator()( const int8_t val ) const
	//	{
	//		( void )( val );
	//		return "char";
	//	}

	//	std::string operator()( const uint32_t val ) const
	//	{
	//		( void )( val );
	//		return "unsigned int";
	//	}

	//	std::string operator()( const int32_t val ) const
	//	{
	//		( void )( val );
	//		return "int";
	//	}

	//	std::string operator()( const uint64_t val ) const
	//	{
	//		( void )( val );
	//		return "unsigned long";
	//	}

	//	std::string operator()( const int64_t val ) const
	//	{
	//		( void )( val );
	//		return "long";
	//	}

	//	std::string operator()( double const& val ) const
	//	{
	//		( void )( val );
	//		return "double";
	//	}

	//	std::string operator()( const float val ) const
	//	{
	//		( void )( val );
	//		return "float";
	//	}

	//	std::string operator()( std::string const& val ) const
	//	{
	//		( void )( val );
	//		return "string";
	//	}

	//	std::string operator()( const bool val ) const
	//	{
	//		( void )( val );
	//		return "bool";
	//	}

	//	std::string operator()( _2Real::CustomDataItem const& val ) const
	//	{
	//		return val.getHumanReadableName();
	//	}

	//};

}