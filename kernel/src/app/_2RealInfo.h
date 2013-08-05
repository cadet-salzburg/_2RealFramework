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

#include "app/_2RealCommon.h"
#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealVersion.h"
#include "policies/_2RealUpdatePolicy.h"
#include "datatypes/_2RealDataField.h"

namespace _2Real
{
	class CustomType;
	class TypeMetadata;
	class BundleMetadata;
	class BlockMetadata;
	class IOMetadata;

	namespace app
	{
		class TypeMetainfo
		{
		public:
			TypeMetainfo();
			explicit TypeMetainfo( std::shared_ptr< const TypeMetadata > );
			bool isValid() const;
			void									getDataFields( DataFields &fields ) const;
			std::pair< std::string, std::string >	getTypename() const;
			std::shared_ptr< CustomType >			makeData() const;
		private:
			std::weak_ptr< const TypeMetadata >			mImpl;
		};

		class InputMetainfo
		{
		public:
			InputMetainfo();
			explicit InputMetainfo( std::shared_ptr< const IOMetadata > );
			bool isValid() const;
			std::string		const& getName() const;
			std::pair< std::string, std::string > const& getTypeName() const;
			TypeMetainfo	getTypeMetainfo() const;
			bool			canExpand() const;
			//bool			canLink() const;
			//bool			isBuffered() const;
			//bool			canTrigger() const;
		private:
			std::weak_ptr< const IOMetadata >			mImpl;
		};

		typedef InputMetainfo InletMetainfo;
		typedef InputMetainfo ParameterMetainfo;

		class OutputMetainfo
		{
		public:
			OutputMetainfo();
			explicit OutputMetainfo( std::shared_ptr< const IOMetadata > );
			bool isValid() const;
			std::string		const& getName() const;
			std::pair< std::string, std::string > const& getTypeName() const;
			TypeMetainfo	getTypeMetainfo() const;
			//bool			canExpand() const;
			//bool			canLink() const;
		private:
			std::weak_ptr< const IOMetadata >			mImpl;
		};

		typedef OutputMetainfo OutletMetainfo;

		class BlockMetainfo
		{
		public:
			BlockMetainfo();
			explicit BlockMetainfo( std::shared_ptr< const BlockMetadata > );
			bool isValid() const;
			std::string const&		getName() const;
			std::string const&		getDescription() const;
			std::string const&		getCategory() const;
			bool					isContext() const;
			bool					needsContext() const;
			void getInletMetainfo( std::vector< InputMetainfo > & ) const;
			void getParameterMetainfo( std::vector< InputMetainfo > & ) const;
			void getOutletMetainfo( std::vector< OutputMetainfo > & ) const;
		private:
			std::weak_ptr< const BlockMetadata >		mImpl;
		};

		class BundleMetainfo
		{
		public:
			BundleMetainfo();
			explicit BundleMetainfo( std::shared_ptr< const BundleMetadata > );
			bool isValid() const;
			std::string const&		getName() const;
			std::string const&		getInstallDirectory() const;
			std::string const&		getDescription() const;
			std::string const&		getAuthor() const;
			std::string const&		getContact() const;
			std::string const&		getCategory() const;
			Version const&			getVersion() const;
			bool					exportsContext() const;						// if the bundle exports a context
			void getExportedBlocks( std::vector< BlockMetainfo > & ) const;
		private:
			std::weak_ptr< const BundleMetadata >		mImpl;
		};
	}
}