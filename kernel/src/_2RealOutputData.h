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

#include "_2RealException.h"

#include "Poco/Any.h"

namespace _2Real
{

	/**
	*
	*/

	class Identifier;
	class OutputDataImpl;

	class OutputData
	{

	public:

		/**
		*	
		*/
		template< typename T >
		T& get(Identifier const& _id) throw(...)
		{
			try
			{
				Poco::Any any = get(_id);
				//might be critical
				return Poco::RefAnyCast< T >(any)
			}
			catch (Poco::BadCastException e)
			{
				throw Exception::failure();
			}
			catch (...)
			{
				throw;
			}
		}

	private:

		/**
		*
		*/
		Poco::Any get(Identifier const& _id) throw(...);

		/**
		*
		*/
		OutputDataImpl		*m_Impl;

	};
}