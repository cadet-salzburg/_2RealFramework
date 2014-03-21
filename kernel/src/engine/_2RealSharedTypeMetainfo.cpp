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

#include "engine/_2RealSharedTypeMetainfo.h"

namespace _2Real
{
	SharedTypeMetainfo::SharedTypeMetainfo( std::string const& name )/* :
		mTypeDescriptor( new CustomTypeDescriptor )*/
	{
		( void )( name );
	}

	std::shared_ptr< CustomDataItem > SharedTypeMetainfo::makeData() const
	{
		// this will copy all fields & default values
		CustomDataItem *item = new CustomDataItem( mTemplate/*, *mTypeDescriptor.get()*/ );
		return std::shared_ptr< CustomDataItem >( item );
	}

	// TODO: missing default value as default argument for value
	void SharedTypeMetainfo::addField( std::string const& fieldName, DataItem const& value )
	{
		DataField field;
		field.mFieldName = fieldName;
		field.mValue = value;

		// TODO: missing recursion!
		//DataFieldDescriptor fieldDescriptor;
		//fieldDescriptor.mFieldName = fieldName;
		// custom type can only be instantiated once name & fields are known
		//fieldDescriptor.mHumanReadableName = boost::apply_visitor( HumanReadableNameVisitor(), value );
		//boost::apply_visitor( SubFieldsVisitor(), value, fieldDescriptor.mFields );

		mTemplate.mDataFields.push_back( field );
		//mTypeDescriptor->mFields.push_back( fieldDescriptor );
	}
}