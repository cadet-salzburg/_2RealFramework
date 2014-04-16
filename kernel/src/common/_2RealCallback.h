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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealCallback_T.h"

namespace _2Real
{
	namespace app
	{
		template< typename TArg >
		class Callback_T
		{

		public:

			typedef void ( _2REAL_CALLBACK *Callback )( void *, TArg );

			Callback_T( Callback cb, void *userData = nullptr )
			{
				mObj.reset( new FunctionCallback_T< TArg >( cb, userData ) );
			}

			template< typename TCallable >
			Callback_T( TCallable &callable, void ( TCallable::*callback )( TArg ) )
			{
				mObj.reset( new MemberCallback_T< TCallable, TArg >( callable, callback ) );
			}

			operator std::shared_ptr< AbstractCallback_T< TArg > > () const
			{
				return mObj;
			}

		private:

			std::shared_ptr< AbstractCallback_T< TArg > >	mObj;

		};

		template< >
		class Callback_T< void >
		{

		public:

			typedef void ( _2REAL_CALLBACK *Callback )( void * );

			Callback_T( Callback cb, void *userData = nullptr )
			{
				mObj.reset( new FunctionCallback_T< void >( cb, userData ) );
			}

			template< typename TCallable >
			Callback_T( TCallable &callable, void ( TCallable::*callback )( void ) )
			{
				mObj.reset( new MemberCallback_T< TCallable, void >( callable, callback ) );
			}

			operator std::shared_ptr< AbstractCallback_T< void > > () const
			{
				return mObj;
			}

		private:

			std::shared_ptr< AbstractCallback_T< void > >	mObj;

		};
	}
}