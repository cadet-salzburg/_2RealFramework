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

#include "_2RealBlockIdentifier.h"
#include "_2RealBundleIdentifier.h"

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include <sstream>
#include <iostream>

#include "Poco/Mutex.h"

namespace Poco
{
	class Path;
}

namespace _2Real
{

	class SafeBool
	{

	public:

		explicit SafeBool( const bool val );

		bool isSet() const;
		bool isUnset() const;
		void set();
		void unset();

	private:

		SafeBool( SafeBool const& src );
		SafeBool& operator=( SafeBool const& src );

		mutable Poco::FastMutex		m_Access;
		bool						m_Bool;

	};

	template< class T > void safeDelete( T*& pVal )
	{
		delete pVal;
		pVal = nullptr;
	}

	template< class T > void safeDeleteArray( T*& pVal )
	{
		delete[] pVal;
		pVal = nullptr;
	}

	const std::string validateName(std::string const& s);
	const std::string pathToName(Poco::Path const& path);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename Datatype >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BundleIdentifier, Datatype > &m)
	{
		if ( m.empty() )
		{
			return out;
		}

		for ( typename std::map< BundleIdentifier, Datatype >::const_iterator it = m.begin(); it != m.end(); ++it )
		{
			out << it->first << " [ " << it->second << " ] \n";
		}
		return out;
	}

	//template< typename Datatype >
	//std::ostream& operator<<(std::ostream &out, typename const std::map< BundleIdentifier, Datatype * > &m)
	//{
	//	if (m.empty())
	//	{
	//		return out;
	//	}

	//	for (typename std::map< BundleIdentifier, Datatype * >::const_iterator it = m.begin(); it != m.end(); ++it)
	//	{
			//if ( it->second == nullptr )
			//{
			//	out << "NULL \n";
			//}
			//else
			//{
			//	out << *( it->second ) << "\n";
			//}
	//	}
	//	return out;
	//}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename Datatype >
	std::ostream& operator<<( std::ostream &out, typename std::map< BlockIdentifier, Datatype > const& m )
	{
		if ( m.empty() )
		{
			return out;
		}

		for ( typename std::map< BlockIdentifier, Datatype >::const_iterator it = m.begin(); it != m.end(); ++it )
		{
			out << it->first << " [ " << it->second << " ] \n";
		}
		return out;
	}

	//template< typename Datatype >
	//std::ostream& operator<<( std::ostream &out, typename std::map< BlockIdentifier, Datatype * > const& m )
	//{
	//	if ( m.empty() )
	//	{
	//		return out;
	//	}

	//	for ( typename std::map< BlockIdentifier, Datatype * >::const_iterator it = m.begin(); it != m.end(); ++it )
	//	{
	//		if ( it->second == nullptr )
	//		{
	//			out << "NULL \n";
	//		}
	//		else
	//		{
	//			out << *( it->second ) << "\n";
	//		}
	//	}
	//	return out;
	//}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename KeyType, typename ValueType >
	ValueType *const getMapElement( KeyType const& key, typename std::map< KeyType, ValueType * > &map )
	{
		typename std::map< KeyType, ValueType * >::iterator it = map.find( key );
		if ( it != map.end() )
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}

	template< typename KeyType, typename ValueType >
	ValueType const* const getMapElement( KeyType const& key, typename std::map< KeyType, ValueType * > const& map )
	{
		typename std::map< KeyType, ValueType * >::const_iterator it = map.find( key );
		if ( it != map.end() )
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}

	template< typename KeyType, typename ValueType >
	ValueType & getMapElement( KeyType const& key, typename std::map< KeyType, ValueType * > &map, std::string const& valueType )
	{
		ValueType *const valPtr = getMapElement< KeyType, ValueType >( key, map );
		if ( valPtr == nullptr )
		{
			std::ostringstream msg;
			msg << valueType << " " << key << " not found";
			throw NotFoundException( msg.str() );
		}
		else
		{
			return *valPtr;
		}
	}

	template< typename KeyType, typename ValueType >
	ValueType const& getMapElement( KeyType const& key, typename std::map< KeyType, ValueType * > const& map, std::string const& valueType )
	{
		ValueType const* const valPtr = getMapElement< KeyType, ValueType >( key, map );
		if ( valPtr == nullptr )
		{
			std::ostringstream msg;
			msg << valueType << " " << key << " not found";
			throw NotFoundException( msg.str() );
		}
		else
		{
			return *valPtr;
		}
	}

}