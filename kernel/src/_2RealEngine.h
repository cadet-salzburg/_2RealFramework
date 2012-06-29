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

#include <string>

namespace _2Real
{

	class EngineImpl;

	namespace app
	{
		class BundleHandle;
	}

	class Engine
	{

		template< typename T >
		friend class SingletonHolder;

	public:

		static Engine& instance();

		/*
		*	sets the base directory, from where all bundles will be searched
		*/
		void setBaseDirectory( std::string const& directory );

		/*
		*	loads the dynamic lib from the given path
		*	if path is relative, it will be interpreted with respect to the base directory
		*	if bundle is already loaded, will throw already exists exception
		*	if library cannot be loaded ( not existing or no metadata ) will throw not found exception
		*/
		app::BundleHandle loadBundle( std::string const& libraryPath );

		//bool isLoaded( std::string const& libraryPath ) const;
		//app::BundleHandle getBundleHandle( std::string const& libraryPath ) const;

	private:

		Engine();
		Engine( Engine const& src );
		~Engine();

		EngineImpl		&m_EngineImpl;

	};


	//class Engine
	//{

	//public:

	//	static Engine& instance();

	//	void setBaseDirectory( std::string const& directory );
	//	const BundleIdentifier load( std::string const& path );
	//	bool isLoaded( std::string const& libraryPath ) const;
	//	const std::string getInfoString( BundleIdentifier const& bundleId );
	//	BundleData const& getBundleData( BundleIdentifier const& bundleId ) const;
	//	BlockData const& getBlockData( BundleIdentifier const& bundleId, std::string const& blockName ) const;

	//private:

	//	template< typename T >
	//	friend class SingletonHolder;

	//	Engine();
	//	Engine(Engine const& src);
	//	~Engine();

	//	EngineImpl								&m_EngineImpl;

	//};

}