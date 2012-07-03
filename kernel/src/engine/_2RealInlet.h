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
//#include "engine/_2RealInletBuffer.h"
#include "helpers/_2RealHandleAble.h"
#include "app/_2RealInletHandle.h"
#include "bundle/_2RealInletHandle.h"

#include <string>

namespace _2Real
{

	namespace app
	{
		class OutletHandle;
	}

	class Inlet : public Parameter, public HandleAble< app::InletHandle >, public HandleAble< bundle::InletHandle >
	{

	public:

		Inlet( AbstractUberBlock &owningBlock, std::string const& name, std::string const& longTypename, std::string const& type );

		//void updateCurrentValue();
		//void updateDataBuffer();

		void linkTo( app::OutletHandle &outlet );
		void unlinkFrom( app::OutletHandle &outlet );

		//void setBufferSize( const unsigned int sz );
		//void setDefaultValue( EngineData const& data );
		//void disableTriggering( TimestampedData const& data );
		//void registerUpdateTrigger( AbstractInletBasedTrigger &trigger );
		//void unregisterUpdateTrigger( AbstractInletBasedTrigger &trigger );

	private:

		TimestampedData		m_CurrentData;
		TimestampedData		m_LastData;
		//InletBuffer		m_Buffer;

	};

}