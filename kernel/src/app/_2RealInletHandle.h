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

#include "helpers/_2RealAny.h"
#include "helpers/_2RealHandle.h"

namespace _2Real
{
	class InletIO;

	namespace app
	{
		class AppData;
		class OutletHandle;

		class InletHandle : private Handle
		{

		public:

			enum InletUpdatePolicy
			{
				ALWAYS,
				OR_NEWER_DATA,
				AND_NEWER_DATA,
			};

			InletHandle();
			InletHandle( InletIO &inletIO );
			~InletHandle();
			InletHandle( InletHandle const& other );
			InletHandle& operator=( InletHandle const& other );

			std::string const&	getName() const;
			std::string const&	getLongTypename() const;
			std::string const&	getTypename() const;

			bool isValid() const;
			bool operator==( InletHandle const& other ) const;
			bool operator!=( InletHandle const& other ) const;
			bool operator<( InletHandle const& other ) const;
			bool operator<=( InletHandle const& other ) const;
			bool operator>( InletHandle const& other ) const;
			bool operator>=( InletHandle const& other ) const;

			void setUpdatePolicy( const InletUpdatePolicy policy );

			void linkTo( OutletHandle &outletHandle );
			void unlinkFrom( OutletHandle &outletHandle );

			// if the inlet is linked, the value might be overwritten of course
			template< typename T >
			void setValue( T const& value )
			{
				setValue( Any( value ) );
			}

			void setValueToString( std::string const& value );

			// returns the inlet's most recent input data
			// updates right before an update() -> stays the same until next update()
			AppData				getCurrentInput() const;

			void				setBufferSize( const unsigned int size );

		protected:

			void invalidate();

		private:

			friend class OutletHandle;

			void				setValue( Any const& data );
			InletIO				*m_InletIO;

		};
	}
}