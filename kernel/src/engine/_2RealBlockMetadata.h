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

#include "engine/_2RealParameterMetadata.h"
#include "engine/_2RealThreadingPolicy.h"

#include <vector>
#include <string>

namespace _2Real
{

	class TypeRegistry;

	class BlockMetadata
	{

	public:

		typedef std::pair< std::string, std::string > BlockId;

		typedef std::vector< InletMetadata const* >						InletMetadatas;
		typedef std::vector< InletMetadata const* >::iterator			InletMetadataIterator;
		typedef std::vector< InletMetadata const* >::const_iterator		InletMetadataConstIterator;

		typedef std::vector< OutletMetadata const* >					OutletMetadatas;
		typedef std::vector< OutletMetadata const* >::iterator			OutletMetadataIterator;
		typedef std::vector< OutletMetadata const* >::const_iterator	OutletMetadataConstIterator;

		//BlockMetadata( TypeRegistry const* reg );
		BlockMetadata( BlockId const& id, TypeRegistry const* reg );
		~BlockMetadata();

		void setDescription( std::string const& description );
		void setCategory( std::string const& category );
		void addInlet( InletMetadata *data );
		void addOutlet( OutletMetadata *data );
		void setThreadingPolicy( ThreadingPolicy const& policy );

		std::string const& getName() const;
		std::string const& getDescription() const;
		std::string const& getCategory() const;

		InletMetadatas const& getInlets() const;
		OutletMetadatas const& getOutlets() const;

		static void performBlockNameCheck( std::string const& name );

	private:

		ThreadingPolicy		m_ThreadingPolicy;

		//std::string			m_Name;
		BlockId				mBlockId;
		std::string			m_Description;
		std::string			m_Category;

		InletMetadatas		m_Inlets;
		OutletMetadatas		m_Outlets;

		TypeRegistry		const* mRegistry;

	};
}