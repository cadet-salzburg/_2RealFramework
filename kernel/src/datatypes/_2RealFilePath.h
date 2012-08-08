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

#include "poco/path.h"

#include <istream>
#include <ostream>
#include <string>

namespace _2Real
{
	class FilePath : private Poco::Path
	{

		friend std::istream & operator>>( std::istream &in, FilePath &path );
		friend std::ostream & operator<<( std::ostream &out, FilePath const& path );

	public:

		FilePath() {}
		FilePath(std::string strPath) : Poco::Path(strPath) {}
		FilePath( FilePath const& src ) : Poco::Path(src) {}

		bool operator==(FilePath const& rhs) const { return (this->toString() == rhs.toString()); }
		bool operator!=(FilePath const& rhs) const { return !(*this == rhs); }

	private:

	};

	inline std::istream & operator>>( std::istream &in, FilePath &path )
	{
		in >> path.toString();  // this is most likely wrong and it should gíve a compile time error
		return in;
	}

	inline std::ostream & operator<<( std::ostream &out, FilePath const& path )
	{
		out << path.toString();
		return out;
	}
}
