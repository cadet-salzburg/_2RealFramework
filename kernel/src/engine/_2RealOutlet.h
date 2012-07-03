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
#include "engine/_2RealTimestampedData.h"
#include "app/_2RealAppData.h"
#include "helpers/_2RealPoco.h"
#include "app/_2RealCallbacks.h"
#include "helpers/_2RealHandleAble.h"
#include "app/_2RealOutletHandle.h"
#include "bundle/_2RealOutletHandle.h"

namespace _2Real
{

	namespace app
	{
		class InletHandle;
	}

	class EngineImpl;

	class Outlet : public Parameter, public HandleAble< app::OutletHandle >, public HandleAble< bundle::OutletHandle >
	{

	public:

		Outlet( AbstractUberBlock &owner, std::string const& name, std::string const& longTypename, std::string const& typeName, EngineData const& emptyData );

		using Parameter::getTypename;
		using Parameter::getLongTypename;
		using Parameter::getName;
		using Parameter::getData;
		using Parameter::getOwningUberBlock;

		void			linkTo( app::InletHandle &inlet );
		void			unlinkFrom( app::InletHandle &Inlet );
		void			registerToNewData( app::OutletCallback &callback );
		void			unregisterFromNewData( app::OutletCallback &callback );

		bool			update();
		EngineData &	getDataForWriting();
		void			discardCurrentUpdate();

		void			addListener( AbstractCallback< TimestampedData > &callback );
		void			removeListener( AbstractCallback< TimestampedData > &callback );

	private:

		EngineImpl										&m_Engine;					// engine is necessary for timestamps
		TimestampedData									m_WriteDataItem;			// the data item that is currently being written
																					// initially, holds an empty ( created by () ) data item of correct type
																					// after the first update, holds a copy of the m_LastDataItem
		bool											m_DiscardCurrent;			// is flagged if the current data should not be sent
		CallbackEvent< TimestampedData >				m_InletEvent;
	};

}