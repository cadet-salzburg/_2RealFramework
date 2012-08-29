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
			virtual Block & create( ContextBlock *const context ) = 0;
			virtual void reset() {}

		};

		template< typename BlockDerived >
		class CreationPolicy
		{

		public:

			virtual ~CreationPolicy() {}
			virtual Block & create( ContextBlock *const context ) = 0;
			virtual void reset() {}

		};

		template< typename BlockDerived >
		class WithContext : public CreationPolicy< BlockDerived >
		{

		public:

			Block & create ( ContextBlock *const context ) { return *( new BlockDerived( *context ) ); }

		};

		template< typename BlockDerived >
		class WithoutContext : public CreationPolicy< BlockDerived >
		{

		public:

			Block & create ( ContextBlock *const context ) { return *( new BlockDerived() ); }

		};

		// not threadsafe!
		template< typename ContextBlockDerived >
		class CreateContext : public CreationPolicy< ContextBlockDerived >
		{

		public:

			CreateContext() : m_Obj( nullptr )		{}

			Block & create( ContextBlock *const context )
			{
				if ( m_Obj == nullptr )
				{
					m_Obj = new ContextBlockDerived();
				}
				return *m_Obj;
			}

			void reset() { m_Obj = nullptr; }

		private:

			ContextBlockDerived			*m_Obj;

		};

		template< typename BlockDerived, template < typename BlockDerived > class CreationPolicy >
		class BlockCreator : public AbstractBlockCreator
		{

		public:

			Block & create( ContextBlock *const context ) { return m_Policy.create( context ); }
			void reset() { m_Policy.reset(); }

		private:

			CreationPolicy< BlockDerived >	m_Policy;

		};
	}
}