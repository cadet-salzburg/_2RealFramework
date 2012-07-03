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

#include "app/_2RealCallbacks.h"
#include "app/_2RealCallbacksInternal.h"
#include "helpers/_2RealPoco.h"
#include "engine/_2RealTimestampedData.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealOutlet.h"

#include <string>

namespace _2Real
{

	class AbstractUberBlock;
	class Inlet;
	class InletBuffer;
	class Outlet;
	class Parameter;

	class AbstractIOManager
	{

	public:

		AbstractIOManager( AbstractUberBlock &owner );
		virtual ~AbstractIOManager();

		std::string const&		getName() const;
		unsigned int			getId() const;

	protected:

		class InletIO
		{
		public:
			Inlet									m_Inlet;
			InletBuffer								m_Buffer;
		};

		typedef std::vector< Inlet * >						InletVector;
		typedef std::vector< Inlet * >::iterator			InletIterator;
		typedef std::vector< Inlet * >::const_iterator		InletConstIterator;

		class OutletIO
		{
		public:
			Outlet									*outlet;
			CallbackEvent< app::AppData const& >	*callbacks;
		};

		typedef std::vector< OutletIO >						OutletVector;
		typedef std::vector< OutletIO >::iterator			OutletIterator;
		typedef std::vector< OutletIO >::const_iterator		OutletConstIterator;

		typedef std::vector< Parameter * >					ParameterVector;
		typedef std::vector< Parameter * >::iterator		ParameterIterator;
		typedef std::vector< Parameter * >::const_iterator	ParameterConstIterator;

		Poco::FastMutex										m_IOAccess;
		AbstractUberBlock									&m_Owner;
		CallbackEvent< std::list< app::AppData > const& >	m_BlockDataChanged;

	};

}