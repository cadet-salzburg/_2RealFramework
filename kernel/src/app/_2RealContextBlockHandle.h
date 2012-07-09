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

#include "helpers/_2RealHandle.h"

namespace _2Real
{
	template< typename T >
	class FunctionBlock;
	
	namespace app
	{
		class BlockInfo;

		class ContextBlockHandle : private Handle
		{

		public:

			ContextBlockHandle();
			ContextBlockHandle( FunctionBlock< ContextBlockHandle > &block );
			~ContextBlockHandle();
			ContextBlockHandle( ContextBlockHandle const& other );
			ContextBlockHandle& operator=( ContextBlockHandle const& other );

			BlockInfo getBlockInfo() const;

			void setUpdateRate( const double updatesPerSecond );

			bool isValid() const;
			bool operator==( ContextBlockHandle const& other ) const;
			bool operator!=( ContextBlockHandle const& other ) const;
			bool operator<( ContextBlockHandle const& other ) const;
			bool operator<=( ContextBlockHandle const& other ) const;
			bool operator>( ContextBlockHandle const& other ) const;
			bool operator>=( ContextBlockHandle const& other ) const;

		protected:

			void invalidate();

		private:

			FunctionBlock< ContextBlockHandle >		*m_Block;

		};

	}
}