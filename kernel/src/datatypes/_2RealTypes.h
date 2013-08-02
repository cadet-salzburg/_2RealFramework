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

#include "helpers/_2RealStringHelpers.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename T > struct Init;

	template< >
	struct Init< char >
	{
		static int defaultValue()
		{
			return static_cast< char >( 0 );
		}
	};

	template< >
	struct Init< unsigned char >
	{
		static int defaultValue()
		{
			return static_cast< char >( 0 );
		}
	};

	template< >
	struct Init< unsigned int >
	{
		static int defaultValue()
		{
			return static_cast< unsigned int >( 0 );
		}
	};

	template< >
	struct Init< int >
	{
		static int defaultValue()
		{
			return 0;
		}
	};

	template< >
	struct Init< float >
	{
		static float defaultValue()
		{
			return 0.f;
		}
	};

	template< >
	struct Init< double >
	{
		static double defaultValue()
		{
			return 0.0;
		}
	};

	template< >
	struct Init< bool >
	{
		static bool defaultValue()
		{
			return false;
		}
	};

	template< >
	struct Init< std::string >
	{
		static std::string defaultValue()
		{
			return std::string();
		}
	};

	template< typename TType >
	struct Init< std::vector< TType > >
	{
		static std::vector< TType > defaultValue()
		{
			return std::vector< TType >();
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename T > struct Name;

	template< >
	struct Name< char >
	{
		static std::string humanReadableName()
		{
			return "char";
		}
	};

	template< >
	struct Name< unsigned char >
	{
		static std::string humanReadableName()
		{
			return "uchar";
		}
	};

	template< >
	struct Name< int >
	{
		static std::string humanReadableName()
		{
			return "int";
		}
	};

	template< >
	struct Name< unsigned int >
	{
		static std::string humanReadableName()
		{
			return "uint";
		}
	};

	template< >
	struct Name< float >
	{
		static std::string humanReadableName()
		{
			return "float";
		}
	};

	template< >
	struct Name< double >
	{
		static std::string humanReadableName()
		{
			return "double";
		}
	};

	template< >
	struct Name< bool >
	{
		static std::string humanReadableName()
		{
			return "bool";
		}
	};

	template< >
	struct Name< std::string >
	{
		static std::string humanReadableName()
		{
			return "string";
		}
	};

	template< typename TType >
	struct Name< std::vector< TType > >
	{
		static std::string humanReadableName()
		{
			return std::string( "vector of " ).append( Name< TType >::humanReadableName() );
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename TType >
	inline void writeTo( std::ostream &out, TType const& v )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
		out << "UNSERIALIZABLE TYPE";
	}

	template< typename TType >
	inline void writeTo( std::ostream &out, typename std::vector< TType > const& v )
	{
		if ( v.empty() ) return;

		typename std::vector< TType >::const_iterator it = v.begin();
		writeTo( out, *it );
		++it;
		for ( ; it != v.end(); ++it )
		{
			out << ", ";
			writeTo( out, *it );
		}
	}

	template< >
	inline void writeTo( std::ostream &out, char const& v )
	{
		out << static_cast< int >( v );
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned char const& v )
	{
		out << static_cast< int >( v );
	}

	template< >
	inline void writeTo( std::ostream &out, short const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned short const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, int const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned int const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, long const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned long const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, bool const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, std::string const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, float const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, double const& v )
	{
		out << v;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename TType >
	inline void readFrom( std::istream &in, TType &v )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	template< typename TType >
	inline void readFrom( std::istream &in, typename std::vector< TType > &v )
	{
		std::string element;

		while ( getline( in, element, ',' ) )
		{
			trim( element );
			TType tmp;
			std::stringstream stream;
			stream << element;
			readFrom( stream, tmp );
			v.push_back( tmp );
		}
	}

	template< >
	inline void readFrom( std::istream &in, char &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned char &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, short &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned short &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, int &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned int &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, long &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned long &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, bool &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, std::string &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, float &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, double &v )
	{
		in >> v;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename TType >
	inline bool isLess( TType const& v1, TType const& v2 )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
		return false;
	}

	template < >
	inline bool isLess( char const& v1, char const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( unsigned char const& v1, unsigned char const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( short const& v1, short const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( unsigned short const& v1, unsigned short const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( int const& v1, int const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( unsigned int const& v1, unsigned int const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( long const& v1, long const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( unsigned long const& v1, unsigned long const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( bool const& v1, bool const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( std::string const& v1, std::string const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( float const& v1, float const& v2 )
	{
		return ( v1 < v2 );
	}

	template < >
	inline bool isLess( double const& v1, double const& v2 )
	{
		return ( v1 < v2 );
	}
}