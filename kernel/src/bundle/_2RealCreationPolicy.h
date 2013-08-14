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

#include "../_2RealBlock.h"

namespace _2Real
{
	namespace bundle
	{
		class AbstractBlockCreator
		{
		public:
			virtual ~AbstractBlockCreator() {}
			virtual std::shared_ptr< _2Real::bundle::Block > create ( std::shared_ptr< _2Real::bundle::ContextBlock > context ) = 0;
			virtual unsigned int getCreationCount() const = 0;
			virtual bool needsContext() const = 0;
		};

		template< typename BlockDerived >
		class CreationPolicy
		{
		public:
			virtual ~CreationPolicy() {}
			virtual std::shared_ptr< _2Real::bundle::Block > create ( std::shared_ptr< _2Real::bundle::ContextBlock > context ) = 0;
			virtual unsigned int getCreationCount() const = 0;
			virtual bool needsContext() const = 0;
		};

		template< typename BlockDerived >
		class WithContext : public CreationPolicy< BlockDerived >
		{
		public:
			WithContext() : mCreationCount( 0 ) {}
			std::shared_ptr< _2Real::bundle::Block > create ( std::shared_ptr< _2Real::bundle::ContextBlock > context )
			{
				++mCreationCount;
				return std::shared_ptr< _2Real::bundle::Block >( new BlockDerived( *( context.get() ) ) );
			}
			unsigned int getCreationCount() const { return mCreationCount; }
			bool needsContext() const { return true; }
		private:
			unsigned int mCreationCount;
		};

		template< typename BlockDerived >
		class WithoutContext : public CreationPolicy< BlockDerived >
		{
		public:
			WithoutContext() : mCreationCount( 0 ) {}
			std::shared_ptr< _2Real::bundle::Block > create( std::shared_ptr< _2Real::bundle::ContextBlock > context )
			{
				++mCreationCount;
				return std::shared_ptr< _2Real::bundle::Block >( new BlockDerived );
			}
			unsigned int getCreationCount() const { return mCreationCount; }
			bool needsContext() const { return false; }
		private:
			unsigned int mCreationCount;
		};

		template< typename ContextBlockDerived >
		class CreateContext : public CreationPolicy< ContextBlockDerived >
		{
		public:
			CreateContext() : mObj(), mCreationCount( 0 ) {}
			std::shared_ptr< _2Real::bundle::Block > create( std::shared_ptr< _2Real::bundle::ContextBlock > context )
			{
				if ( mObj.expired() )
				{
					std::shared_ptr< ContextBlockDerived > shared( new ContextBlockDerived );
					mObj = shared;
					++mCreationCount;
					return shared;
				}
				else
				{
					std::shared_ptr< ContextBlockDerived > shared = mObj.lock();
					return shared;
				}
			}
			unsigned int getCreationCount() const { return mCreationCount; }
			bool needsContext() const { return false; }
		private:
			std::weak_ptr< ContextBlockDerived >		mObj;
			unsigned int								mCreationCount;
		};

		template< typename BlockDerived, template < typename BlockDerived > class CreationPolicy >
		class BlockCreator : public AbstractBlockCreator
		{
		public:
			std::shared_ptr< _2Real::bundle::Block > create( std::shared_ptr< _2Real::bundle::ContextBlock > context )
			{
				return mPolicy.create( context );
			}
			unsigned int getCreationCount() const { return mPolicy.getCreationCount(); }
			bool needsContext() const { return mPolicy.needsContext(); }
		private:
			CreationPolicy< BlockDerived >	mPolicy;
		};
	}
}