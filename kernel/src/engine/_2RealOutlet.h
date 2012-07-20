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

#include "engine/_2RealParameter.h"
#include "bundle/_2RealOutletHandle.h"
#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"

namespace _2Real
{
	class EngineImpl;

	class Outlet : private Parameter, private NonCopyable< Outlet >, private Identifiable< Outlet >, private Handleable< Outlet, bundle::OutletHandle >
	{

	public:

		Outlet( AbstractUberBlock &owningBlock, std::string const& name, std::string const& longTypename, std::string const& typeName, Any const& emptyData );

		using Handleable< Outlet, bundle::OutletHandle >::getHandle;
		using Handleable< Outlet, bundle::OutletHandle >::registerHandle;
		using Handleable< Outlet, bundle::OutletHandle >::unregisterHandle;

		using Identifiable< Outlet >::getFullName;
		using Identifiable< Outlet >::getName;

		using Parameter::getTypename;
		using Parameter::getLongTypename;
		using Parameter::getData;

		bool			synchronize();
		Any &			getWriteableData();
		void			discardCurrentUpdate();
		AbstractUberBlock & getOwningUberBlock();

	private:

		EngineImpl				&m_Engine;
		AbstractUberBlock		&m_OwningUberBlock;
		bool					m_DiscardCurrent;

	};
}