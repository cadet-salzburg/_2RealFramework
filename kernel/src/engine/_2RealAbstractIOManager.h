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

#include "helpers/_2RealEvent.h"
#include "helpers/_2RealPoco.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealHandleable.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"

#include <list>
#include <vector>

namespace _2Real
{

	namespace bundle
	{
		class BlockHandle;
		class InletHandle;
		class OutletHandle;
	}

	namespace app
	{
		class AppData;
		class InletHandle;
		class OutletHandle;
	}

	class TimestampedData;
	class AbstractUberBlock;
	class Inlet;
	class InletBuffer;
	class Outlet;
	class ParameterMetadata;

	class InletIO : private NonCopyable< InletIO >, private Handleable< app::InletHandle >
	{

	public:

		using Handleable< app::InletHandle >::getHandle;
		using Handleable< app::InletHandle >::registerHandle;
		using Handleable< app::InletHandle >::unregisterHandle;

		InletIO( AbstractUberBlock &owner, ParameterMetadata const& data );
		~InletIO();
		Inlet													*m_Inlet;
		InletBuffer												*m_Buffer;

	};

	class OutletIO : private NonCopyable< InletIO >, private Handleable< app::OutletHandle >
	{

	public:

		using Handleable< app::OutletHandle >::getHandle;
		using Handleable< app::OutletHandle >::registerHandle;
		using Handleable< app::OutletHandle >::unregisterHandle;

		OutletIO( AbstractUberBlock &owner, ParameterMetadata const& data );
		~OutletIO();
		Outlet													*m_Outlet;
		CallbackEvent< app::AppData const& >					*m_AppEvent;
		CallbackEvent< TimestampedData const& >					*m_InletEvent;

	};

	typedef std::vector< app::InletHandle >							AppInletHandles;
	typedef std::vector< app::InletHandle >::iterator				AppInletHandleIterator;
	typedef std::vector< app::InletHandle >::const_iterator			AppInletHandleConstIterator;

	typedef std::vector< app::OutletHandle >						AppOutletHandles;
	typedef std::vector< app::OutletHandle >::iterator				AppOutletHandleIterator;
	typedef std::vector< app::OutletHandle >::const_iterator		AppOutletHandleConstIterator;

	typedef std::vector< bundle::InletHandle >						BundleInletHandles;
	typedef std::vector< bundle::InletHandle >::iterator			BundleInletHandleIterator;
	typedef std::vector< bundle::InletHandle >::const_iterator		BundleInletHandleConstIterator;

	typedef std::vector< bundle::OutletHandle >						BundleOutletHandles;
	typedef std::vector< bundle::OutletHandle >::iterator			BundleOutletHandleIterator;
	typedef std::vector< bundle::OutletHandle >::const_iterator		BundleOutletHandleConstIterator;

	class AbstractIOManager : private NonCopyable< AbstractIOManager >
	{

	public:

		typedef std::vector< InletIO * >							InletVector;
		typedef std::vector< InletIO * >::iterator					InletIterator;
		typedef std::vector< InletIO * >::const_iterator			InletConstIterator;

		typedef std::vector< OutletIO * >							OutletVector;
		typedef std::vector< OutletIO * >::iterator					OutletIterator;
		typedef std::vector< OutletIO * >::const_iterator			OutletConstIterator;

		AbstractIOManager( AbstractUberBlock &owner );
		virtual ~AbstractIOManager();
		const std::string getName() const;

	protected:

		AbstractUberBlock											&m_Owner;
		Poco::FastMutex												m_IOAccess;
		CallbackEvent< std::list< app::AppData > const& >			m_AppEvent;

	};

}