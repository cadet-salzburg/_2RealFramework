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

#include "_2RealTypeTable.h"

namespace _2Real
{
	TypeTable::TypeTable() : m_TypeTable()
	{
		this->insertType< char >("char");
		this->insertType< unsigned char >("unsigned char");
		this->insertType< short >("short");
		this->insertType< unsigned short >("unsigned short");
		this->insertType< int >("int");
		this->insertType< unsigned int >("unsigned int");
		this->insertType< long >("long");
		this->insertType< unsigned long >("unsigned long");
		this->insertType< float >("float");
		this->insertType< double >("double");
		this->insertType< bool >("bool");
		this->insertType< std::string >("string");
		//this->insertType< Image< float, 2 > >("image2D_ushort");
		//this->insertType< Image< unsigned short, 2 > >("image2D_float");
		//this->insertType< ImageBuffer >("imagebuffer");
	}

	std::map< std::string, std::string > TypeTable::getTable()
	{
		return m_TypeTable;
	}
}