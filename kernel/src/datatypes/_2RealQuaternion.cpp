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

#include "datatypes/_2RealQuaternion.h"

namespace _2Real
{
	std::istream & operator>>( std::istream &in, Quaternion &quaternion )
	{
		in >> quaternion.m_X >> quaternion.m_Y >> quaternion.m_Z >> quaternion.m_Label >> quaternion.m_Id;
		return in;
	}

	std::ostream & operator<<( std::ostream &out, Quaternion const& quaternion )
	{
		out << quaternion.m_X << " " << quaternion.m_Y << " " << quaternion.m_Z << " " << quaternion.m_W << " " << quaternion.m_Label << " " << quaternion.m_Id;
		return out;
	}
}