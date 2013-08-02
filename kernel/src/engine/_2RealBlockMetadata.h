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

#include "engine/_2RealIOMetadata.h"
#include "policies/_2RealThreadingPolicy.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class TypeRegistry;

	class BlockMetadata
	{
	public:

		typedef std::pair< std::string, std::string > BlockId;

		typedef std::vector< std::shared_ptr< IOMetadata > >						IOMetadatas;
		typedef std::vector< std::shared_ptr< IOMetadata > >::iterator				IOMetadataIterator;
		typedef std::vector< std::shared_ptr< IOMetadata > >::const_iterator		IOMetadataConstIterator;

		BlockMetadata( BlockId const&, TypeRegistry *, bool isContext, bool needsContext );

		void setDescription( std::string const& description );
		void setCategory( std::string const& category );
		void addInlet( std::shared_ptr< IOMetadata >, std::string const& );
		void addOutlet( std::shared_ptr< IOMetadata >, std::string const& );
		void addParameter( std::shared_ptr< IOMetadata >, std::string const& );
		void setThreadingPolicy( ThreadingPolicy const& policy );

		std::string const& getName() const;
		std::string const& getDescription() const;
		std::string const& getCategory() const;
		bool isContext() const;
		bool needsContext() const;
		ThreadingPolicy getThreadingPolicy() const;
		IOMetadatas const& getInlets() const;
		IOMetadatas const& getOutlets() const;
		IOMetadatas const& getParameters() const;

	private:

		BlockId				mBlockId;
		std::string			mDescription;
		std::string			mCategory;
		bool				mIsContext;
		bool				mNeedsContext;
		ThreadingPolicy		mThreadingPolicy;

		IOMetadatas			mInlets;
		IOMetadatas			mOutlets;
		IOMetadatas			mParameters;

		TypeRegistry		const* mRegistry;
	};
}