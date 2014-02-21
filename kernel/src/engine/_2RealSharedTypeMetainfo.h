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
#include "helpers/_2RealCallback_T.h"
#include "helpers/_2RealEvent_T.h"

namespace _2Real
{
	class CustomData;
	class TypeCollection;
	class AbstractDataFieldMetainfo;
	class AbstractFieldDescriptor;

	class SharedTypeMetainfo : public std::enable_shared_from_this< SharedTypeMetainfo >
	{

	public:

		SharedTypeMetainfo( std::string const& name );
		~SharedTypeMetainfo();

		std::shared_ptr< CustomData > makeData() const;

		std::string const&		getName() const;

		void addField( std::string const& fieldName, std::string const& typeName, std::shared_ptr< const AbstractFieldDescriptor > );

		bool performExport();
		bool finalize();

		// remove listener.... removal renders all types invalid
		///////////////////////////////////////////////////////
		template< typename TCallable >
		void registerToRemove( TCallable &callable, void ( TCallable::*callback )( std::shared_ptr< const SharedTypeMetainfo > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const SharedTypeMetainfo > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const SharedTypeMetainfo > >( callable, callback ) );
			mRemoveNotifier.addListener( listener );
		}
		template< typename TCallable >
		void unregisterFromRemove( TCallable &callable, void ( TCallable::*callback )( std::shared_ptr< const SharedTypeMetainfo > ) )
		{
			std::shared_ptr< AbstractCallback_T< std::shared_ptr< const SharedTypeMetainfo > > > listener( new MemberCallback_T< TCallable, std::shared_ptr< const SharedTypeMetainfo > >( callable, callback ) );
			mRemoveNotifier.removeListener( listener );
		}
		///////////////////////////////////////////////////////

	private:

		SharedTypeMetainfo( SharedTypeMetainfo const& );
		SharedTypeMetainfo& operator=( SharedTypeMetainfo const& );

		typedef std::vector< std::pair< std::string, std::shared_ptr< const AbstractDataFieldMetainfo > > >		FieldDescriptions;

		// type name, as set by the bundle
		std::string												mName;
		// the datafields
		FieldDescriptions										mFields;
		// once this flag becomes true, it isno longer possible to change the type
		bool													mIsFinal;
		// the 'global' type registry w. framework types
		std::weak_ptr< TypeCollection >							mGlobalTypes;
		// the local type registry, w. library-specific types
		std::weak_ptr< TypeCollection >							mLocalTypes;
		// notifies all listeners when the type is removed ( lib unloaded )
		Event_T< std::shared_ptr< const SharedTypeMetainfo > >	mRemoveNotifier;

	};

}