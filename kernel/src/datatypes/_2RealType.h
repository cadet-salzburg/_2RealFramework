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

namespace _2Real
{
	class Type
	{

	public:

		enum Code
		{
			EMPTY,
			BYTE,
			UBYTE,
			SHORT,
			USHORT,
			INT,
			UINT,
			LONG,
			ULONG,
			FLOAT,
			DOUBLE,
			BOOL,
			STRING,
			VECTOR,
			LIST,
			IMAGE,
			NUMBER,
			POINT,
			SKELETON,
			RIGIDBODY,
			QUATERNION
		};

		Type( Code const& code ) : m_Code( code ) {}
		Code getCode() const { return m_Code; }
		bool operator==( Type const& rhs ) const { return m_Code == rhs.m_Code; }
		bool operator!=( Type const& rhs ) const { return m_Code != rhs.m_Code; }

	private:

		Code		const m_Code;

	};
}
