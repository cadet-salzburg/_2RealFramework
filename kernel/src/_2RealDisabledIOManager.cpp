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

#include "_2RealDisabledIOManager.h"
#include "_2RealException.h"
#include "_2RealAbstractUberBlock.h"

#include <sstream>

namespace _2Real
{

	DisabledIO::DisabledIO(AbstractUberBlock &owner) :
		AbstractIOManager(owner)
	{
	}

	DisabledIO::~DisabledIO()
	{
	}

	void DisabledIO::clear()
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::registerToNewData(std::string const& outName, OutletCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::unregisterFromNewData(std::string const& outName, OutletCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::registerToNewData(std::string const& outName, AbstractOutletCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::unregisterFromNewData(std::string const& outName, AbstractOutletCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::registerToNewData(OutputCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::unregisterFromNewData(OutputCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::registerToNewData(AbstractOutputCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::unregisterFromNewData(AbstractOutputCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	EngineData const& DisabledIO::getValue(std::string const& paramName) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	std::string const& DisabledIO::getTypename(std::string const& paramName) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	std::string const& DisabledIO::getLongTypename(std::string const& paramName) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::setValue(std::string const& paramName, TimestampedData const& value)
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	Inlet const& DisabledIO::getInlet( std::string const& name ) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	Outlet const& DisabledIO::getOutlet( std::string const& name ) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	SetupParameter const& DisabledIO::getSetupParameter( std::string const& name ) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

}