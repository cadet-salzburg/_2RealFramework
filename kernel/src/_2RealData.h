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
#include "_2RealIdentifier.h"
#include "_2RealDataPacket.h"

#include "Poco/Any.h"
#include "Poco/SharedPtr.h"

namespace _2Real
{

	/**
	*	output data received by a container
	*/
	class Data
	{

	public:

		/**
		*	returns the value received from slot _id
		*/
		template< typename T >
		T get(unsigned int const& _id) throw(...)
		{
			try
			{
				Poco::SharedPtr< Poco::Any > any = get(_id);
				return Poco::AnyCast< T >(*(any.get()));
			}
			catch (Poco::BadCastException e)
			{
				throw Exception("the datatype does not match the output slot");
			}
			catch (Exception &e)
			{
				throw e;
			}
		}

		/**
		*	returns sender's identifier
		*/
		const Identifier id() const;

	private:

		friend class ApplicationCallback;

		/**
		*
		*/
		Data(Poco::SharedPtr< DataPacket > _data, Identifier const& _id);

		/**
		*
		*/
		Poco::SharedPtr< Poco::Any > get(unsigned int const& _id) throw(...);

		/**
		*
		*/
		Poco::SharedPtr< DataPacket >	m_Data;

		/**
		*	
		*/
		Identifier						m_ID;

	};
}