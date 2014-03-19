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

#include "engine/_2RealSharedServiceInletMetainfo.h"

namespace _2Real
{

	SharedServiceInletMetainfo::SharedServiceInletMetainfo( std::string const& name ) :
		SharedServiceIoSlotMetainfo( name ),
		mIsMultiInlet( false )
	{
	}

	SharedServiceInletMetainfo::~SharedServiceInletMetainfo()
	{
	}

	void SharedServiceInletMetainfo::setMultiInlet( const bool isMulti )
	{
		mIsMultiInlet = isMulti;
	}

	bool SharedServiceInletMetainfo::isMultiInlet() const
	{
		return mIsMultiInlet;
	}

	std::shared_ptr< SharedServiceInletMetainfo > SharedServiceInletMetainfo::clone() const
	{
		std::shared_ptr< SharedServiceInletMetainfo > result( new SharedServiceInletMetainfo( getName() ) );

		result->setDatatype( getDatatype() );
		result->setDescription( getDescription() );
		result->mIsMultiInlet = mIsMultiInlet;

		return result;
	}

}