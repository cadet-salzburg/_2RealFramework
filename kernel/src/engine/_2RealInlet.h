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
#include "bundle/_2RealInletHandle.h"
#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealNonCopyable.h"

#include <string>

namespace _2Real
{
	class EngineImpl;

	class Inlet : private Parameter, private NonCopyable< Inlet >, private Handleable< bundle::InletHandle >
	{

	public:

		Inlet( AbstractUberBlock &owningBlock, std::string const& name, std::string const& longTypename, std::string const& type );

		using Handleable< bundle::InletHandle >::getHandle;
		using Handleable< bundle::InletHandle >::registerHandle;
		using Handleable< bundle::InletHandle >::unregisterHandle;

		using Parameter::getTypename;
		using Parameter::getLongTypename;
		using Parameter::getName;
		using Parameter::getOwningUberBlock;
		using Parameter::getData;

		void setData( TimestampedData const& data );

	private:

		EngineImpl		&m_Engine;

	};
}