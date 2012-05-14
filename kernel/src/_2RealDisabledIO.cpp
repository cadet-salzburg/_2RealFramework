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

#include "_2RealDisabledIO.h"
#include "_2RealException.h"
#include "_2RealAbstractBlock.h"

#include <sstream>

namespace _2Real
{

	DisabledIO::DisabledIO(AbstractBlock &owner) :
		AbstractIOManager(owner)
	{
	}

	DisabledIO::~DisabledIO()
	{
		clear();
	}

	void DisabledIO::clear()
	{
	}

	//void DisabledIO::addParam(ParameterMetadata const& meta)
	//{
	//	std::ostringstream msg;
	//	msg << "this entity does not allow io";
	//	throw _2Real::Exception(msg.str());
	//}

	//void DisabledIO::addInlet(ParameterMetadata const& meta)
	//{
	//	std::ostringstream msg;
	//	msg << "this entity does not allow io";
	//	throw _2Real::Exception(msg.str());
	//}

	//void DisabledIO::addOutlet(ParameterMetadata const& meta, Poco::Timestamp const& timestamp)
	//{
	//	std::ostringstream msg;
	//	msg << "this entity does not allow io";
	//	throw _2Real::Exception(msg.str());
	//}

	void DisabledIO::registerToNewData(std::string const& outName, DataCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::unregisterFromNewData(std::string const& outName, DataCallback callback, void *userData )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::registerToNewData(std::string const& outName, AbstractDataCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::unregisterFromNewData(std::string const& outName, AbstractDataCallbackHandler &handler )
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	const EngineData DisabledIO::getValue(std::string const& paramName) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	std::string const& DisabledIO::getKey(std::string const& paramName) const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::setValue(std::string const& paramName, Data const& value)
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::insertValue(std::string const& paramName, Data const& value)
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::linkWith(std::string const& inlet, AbstractBlock &out, std::string const& outlet)
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	InletMap const& DisabledIO::getInlets() const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	OutletMap const& DisabledIO::getOutlets() const
	{
		std::ostringstream msg;
		msg << "this entity does not allow io";
		throw _2Real::Exception(msg.str());
	}

	void DisabledIO::subBlockAdded(AbstractBlock &subBlock)
	{
	}

	void DisabledIO::subBlockRemoved(AbstractBlock &subBlock)
	{
	}

	void DisabledIO::uberBlockAdded(AbstractBlock &uberBlock)
	{
	}

	void DisabledIO::uberBlockRemoved(AbstractBlock &uberBlock)
	{
	}

}