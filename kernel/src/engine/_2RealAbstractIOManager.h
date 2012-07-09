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

#include "engine/_2RealTimestampedData.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealOutlet.h"
#include "app/_2RealCallbacks.h"
#include "helpers/_2RealEvent.h"
#include "helpers/_2RealCallback.h"
#include "helpers/_2RealPoco.h"

#include <string>

namespace _2Real
{

	class AbstractUberBlock;
	class Inlet;
	class InletBuffer;
	class Outlet;
	class Parameter;
	class ParameterData;

	class InletIO : public Handleable< app::InletHandle >
	{
	public:
		InletIO( AbstractUberBlock &owner, ParameterData const& data );
		~InletIO();
		Inlet													*m_Inlet;
		InletBuffer												*m_Buffer;
	};

	class OutletIO : public Handleable< app::OutletHandle >
	{
	public:
		OutletIO( AbstractUberBlock &owner, ParameterData const& data );
		~OutletIO();
		Outlet													*m_Outlet;
		CallbackEvent< app::AppData const& >					*m_AppEvent;
		CallbackEvent< TimestampedData const& >					*m_InletEvent;
	};

	typedef std::vector< app::InletHandle >						AppInletHandles;
	typedef std::vector< app::InletHandle >::iterator			AppInletHandleIterator;
	typedef std::vector< app::InletHandle >::const_iterator		AppInletHandleConstIterator;

	typedef std::vector< app::OutletHandle >					AppOutletHandles;
	typedef std::vector< app::OutletHandle >::iterator			AppOutletHandleIterator;
	typedef std::vector< app::OutletHandle >::const_iterator	AppOutletHandleConstIterator;

	typedef std::vector< bundle::InletHandle >					BundleInletHandles;
	typedef std::vector< bundle::InletHandle >::iterator		BundleInletHandleIterator;
	typedef std::vector< bundle::InletHandle >::const_iterator	BundleInletHandleConstIterator;

	typedef std::vector< bundle::OutletHandle >					BundleOutletHandles;
	typedef std::vector< bundle::OutletHandle >::iterator		BundleOutletHandleIterator;
	typedef std::vector< bundle::OutletHandle >::const_iterator	BundleOutletHandleConstIterator;

	class AbstractIOManager
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

		std::string const&		getName() const;
		unsigned int			getId() const;

	protected:

		AbstractUberBlock											&m_Owner;
		Poco::FastMutex												m_IOAccess;
		CallbackEvent< std::list< app::AppData > const& >			m_AppEvent;

	};

}