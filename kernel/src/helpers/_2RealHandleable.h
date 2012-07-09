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

#include "helpers/_2RealException.h"
#include "helpers/_2RealHandle.h"

#include <set>
#include <iostream>
#include <sstream>

namespace _2Real
{

	class AbstractHandleable
	{

	public:

		virtual ~AbstractHandleable();
		void registerHandle( Handle &handle );
		void unregisterHandle( Handle &handle );

	private:

		typedef std::set< Handle * >				Handles;
		typedef std::set< Handle * >::iterator		HandleIterator;
		Handles										m_Handles;

	};

	template< typename THandle >
	class Handleable : public virtual AbstractHandleable
	{

	protected:

		template< typename TObj >
		Handleable( TObj &obj ) :
			m_Handle( new THandle( obj ) )
		{
		}

		~Handleable()
		{
			delete m_Handle;
		}

		THandle& getHandle() const
		{
			return *m_Handle;
		}

	private:

		THandle		*m_Handle;

	};

}