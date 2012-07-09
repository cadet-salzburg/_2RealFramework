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

#include <list>

namespace _2Real
{
	template< typename T >
	class ListInitializer
	{

	public:

		ListInitializer( T const& val )
		{
			m_List.push_back( val );
		}

		ListInitializer& operator()( T const& val )
		{
			m_List.push_back( val );
			return *this;
		}

		operator std::list< T > const& () const
		{
			return m_List;
		}

	private:

		std::list< T >		m_List;

	};
}