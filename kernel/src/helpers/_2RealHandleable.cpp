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

#include "helpers\_2RealHandleable.h"

#include <sstream>

using std::ostringstream;
using std::string;

namespace _2Real
{

	AbstractHandleable::~AbstractHandleable()
	{
		for ( HandleIterator it = m_Handles.begin(); it != m_Handles.end(); ++it )
		{
			( *it )->invalidate();
		}
		m_Handles.clear();
	}

	void AbstractHandleable::registerHandle( Handle &handle )
	{
		m_Handles.insert( &handle );
	}

	void AbstractHandleable::unregisterHandle( Handle &handle )
	{
		HandleIterator it = m_Handles.find( &handle );
		if ( it != m_Handles.end() )
		{
			m_Handles.erase( it );
		}
	}

}