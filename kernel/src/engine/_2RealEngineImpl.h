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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealPath.h"

#include "common/_2RealThreadpoolPolicy.h"

namespace _2Real
{
	class TimerCollection;
	class ThreadpoolCollection;
	class TypeCollection;
	class BundleCollection;
	class LinkCollection;

	class BundleImpl;
	class BundleMetainfoImpl;
	class TimerImpl;
	class ThreadpoolImpl_I;
	class InletImpl;
	class OutletImpl;
	class Link;
	class TypeMetainfoImpl_I;

	class EngineImpl : public std::enable_shared_from_this< EngineImpl >
	{

	public:

		EngineImpl();
		~EngineImpl();
		
		EngineImpl( EngineImpl const& other ) = delete;
		EngineImpl( EngineImpl && other ) = delete;
		EngineImpl& operator=( EngineImpl const& other ) = delete;
		EngineImpl& operator=( EngineImpl && other ) = delete;

		typedef std::pair< std::shared_ptr< BundleImpl >, std::shared_ptr< const BundleMetainfoImpl > > LoadedBundle;

		void									clear();
		LoadedBundle							loadLibrary( std::string const& filename );
		Path									getBundleDirectory() const;
		std::shared_ptr< TimerImpl >			createTimer( const double fps );
		std::shared_ptr< ThreadpoolImpl_I >		createThreadpool( const ThreadpoolPolicy );
		std::shared_ptr< Link >					createLink( std::shared_ptr< InletImpl >, std::shared_ptr< OutletImpl > );
		std::shared_ptr< TypeMetainfoImpl_I >	getTypeMetainfo( std::string const& name ) const;

	private:

		std::shared_ptr< TimerCollection >		mTimerCollection;
		std::shared_ptr< ThreadpoolCollection >	mThreadpoolCollection;
		std::shared_ptr< TypeCollection >		mTypeCollection;
		std::shared_ptr< BundleCollection >		mBundleCollection;
		std::shared_ptr< LinkCollection >		mLinkCollection;
		
	};

}