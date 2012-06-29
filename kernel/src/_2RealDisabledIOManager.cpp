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

	DisabledIOManager::DisabledIOManager(AbstractUberBlock &owner) :
		AbstractIOManager(owner)
	{
	}

	void DisabledIOManager::registerToNewData(std::string const& outName, app::OutletDataCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIOManager::unregisterFromNewData(std::string const& outName, app::OutletDataCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIOManager::registerToNewData(std::string const& outName, app::AbstractOutletDataCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIOManager::unregisterFromNewData( std::string const& outName, app::AbstractOutletDataCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIOManager::registerToNewData( app::BlockDataCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIOManager::unregisterFromNewData( app::BlockDataCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIOManager::registerToNewData( app::AbstractBlockDataCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIOManager::unregisterFromNewData( app::AbstractBlockDataCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	Inlet & DisabledIOManager::getInlet( std::string const& name )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	Outlet & DisabledIOManager::getOutlet( std::string const& name )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	Parameter & DisabledIOManager::getParameter( std::string const& name )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

}