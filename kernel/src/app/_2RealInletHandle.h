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

#include "datatypes/_2RealCustomData.h"
#include "engine/_2RealInletPolicy.h"

namespace _2Real
{
	class AbstractInletIO;

	namespace app
	{
		class OutletHandle;
		class BlockHandle;

		class InletHandle
		{

		public:

			InletHandle();
			InletHandle( AbstractInletIO &inletIO );
			~InletHandle();
			InletHandle( InletHandle const& other );
			InletHandle& operator=( InletHandle const& other );

			bool isValid() const;
			void invalidate();
			bool operator==( InletHandle const& other ) const;
			bool operator!=( InletHandle const& other ) const;
			bool operator<( InletHandle const& other ) const;
			bool operator<=( InletHandle const& other ) const;
			bool operator>( InletHandle const& other ) const;
			bool operator>=( InletHandle const& other ) const;

			std::string const&	getName() const;
			BlockHandle			getOwningBlock();

			void				setUpdatePolicy( InletPolicy const& policy );
			void				setBufferSize( const unsigned int size );

			bool				link( OutletHandle &outletHandle );
			void				unlinkFrom( OutletHandle &outletHandle );

			std::shared_ptr< const CustomType >		getCurrentData() const;

			// if the inlet is linked, the value might be overwritten of course
			//template< typename TData >
			//void setValue( TData const& value )
			//{
			//	setValue( Any( value ) );
			//}

			//void setValueToString( std::string const& value );

			// returns the inlet's most recent input data
			// updates right before an update() -> stays the same until next update()
			//AppData				getCurrentInput() const;

			// multi-inlet related stuff -> each of those functions has a chance of failing

			bool isMultiInlet() const;
			unsigned int getSize() const;
			InletHandle operator[]( const unsigned int index );
			InletHandle add();
			void remove( InletHandle &handle );

		private:

			friend class OutletHandle;

			//void				setValue( Any const& data );
			AbstractInletIO		*m_InletIO;

		};
	}
}
