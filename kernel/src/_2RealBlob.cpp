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

#include "_2RealBlob.h"

namespace _2Real
{
	IMPLEMENT_EXCEPTION(BlobException, Exception, "Blob exception")

	template< typename DataType >
	void Blob< DataType >::copydata(DataType const*data, unsigned int size)
	{
		m_Size = size;
		delete[] m_Data;
		m_Data = NULL;
		if (m_Size > 0)
		{
			m_Data = new DataType[m_Size];
			if (m_Data == NULL)
			{
				std::stringstream txt;
				txt << "memory allocation for " << m_Size << " bytes failed!";
				m_Size = 0;
				throw BlobException(txt.str());
			}
			memcpy(m_Data, data, m_Size);
		}
	}
}