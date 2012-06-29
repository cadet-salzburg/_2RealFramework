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

#include "_2RealParameter.h"
#include "_2RealTimestampedData.h"
#include "app/_2RealAppData.h"
#include "_2RealPoco.h"
#include "app/_2RealCallbacksInternal.h"

namespace _2Real
{

	namespace app
	{
		class InletHandle;
	}

	class InletBuffer;
	class EngineImpl;

	class Outlet : public Parameter
	{

	public:

		Outlet( AbstractUberBlock &owner, std::string const& name, std::string const& longTypename, std::string const& typeName, EngineData const& emptyData );

		using Parameter::getTypename;
		using Parameter::getLongTypename;
		using Parameter::getName;
		using Parameter::getOwningUberBlock;
		using Parameter::getData;

		void linkTo( app::InletHandle &inlet );
		void unlinkFrom( app::InletHandle &Inlet );

		void				update();
		void				discardCurrentUpdate();
		EngineData &		getDataForWriting();

		void				addInletListener( InletBuffer &buffer );
		void				removeInletListener( InletBuffer &buffer );

		void				registerToNewData( app::OutletDataCallback callback, void *userData );
		void				unregisterFromNewData( app::OutletDataCallback callback, void *userData );
		void				registerToNewData( app::AbstractOutletDataCallbackHandler &handler );
		void				unregisterFromNewData( app::AbstractOutletDataCallbackHandler &handler );

	private:

		EngineImpl										&m_Engine;					// engine is necessary for timestamps

		TimestampedData									m_WriteDataItem;			// the data item that is currently being written
																					// initially, holds an empty ( created by () ) data item of correct type
																					// after the first update, holds a copy of the m_LastDataItem
		bool											m_DiscardCurrent;			// is flagged if the current data should not be sent

		mutable Poco::FastMutex							m_Access;
		mutable Poco::BasicEvent< TimestampedData >		m_InletEvent;

	};

}