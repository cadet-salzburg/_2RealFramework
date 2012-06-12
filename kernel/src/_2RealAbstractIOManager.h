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

#include "_2RealCallbacks.h"
#include "_2RealCallbacksInternal.h"

#include <map>
#include <string>

namespace _2Real
{

	class EngineData;
	class InletHandle;
	class OutletHandle;
	class AbstractBlock;
	class Inlet;
	class Outlet;
	class SetupParameter;
	class TimestampedData;

	typedef std::map< std::string, Inlet * >		InletMap;
	typedef std::map< std::string, Outlet * >		OutletMap;
	typedef std::map< std::string, SetupParameter * >	ParamMap;

	class AbstractIOManager
	{

	public:

		AbstractIOManager(AbstractBlock &owner);
		virtual ~AbstractIOManager();

		std::string const& getName() const;
		const unsigned int getId() const;

		virtual void					clear() = 0;
		//virtual void					addParam(ParameterData const& meta) = 0;
		//virtual void					addInlet(ParameterData const& meta) = 0;
		//virtual void					addOutlet(ParameterData const& meta, Poco::Timestamp const& timestamp) = 0;
		virtual void					registerToNewData( std::string const& outName, OutletCallback callback, void *userData ) = 0;
		virtual void					unregisterFromNewData( std::string const& outName, OutletCallback callback, void *userData ) = 0;
		virtual void					registerToNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler) = 0;
		virtual void					unregisterFromNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler) = 0;
		virtual void					registerToNewData(OutputCallback callback, void *userData ) = 0;
		virtual void					unregisterFromNewData(OutputCallback callback, void *userData ) = 0;
		virtual void					registerToNewData(AbstractOutputCallbackHandler &handler) = 0;
		virtual void					unregisterFromNewData(AbstractOutputCallbackHandler &handler) = 0;

		virtual const EngineData		getValue(std::string const& paramName) const = 0;
		virtual std::string const&		getKey(std::string const& paramName) const = 0;
		virtual void					setValue(std::string const& paramName, TimestampedData const& value) = 0;
		virtual void					insertValue(std::string const& paramName, TimestampedData &value) = 0;

		virtual void					linkWith(std::string const& inlet, AbstractBlock &out, std::string const& outlet) = 0;

		virtual InletMap const&			getInlets() const = 0;
		virtual OutletMap const&		getOutlets() const = 0;

		virtual void					subBlockAdded(AbstractBlock &subBlock) = 0;
		virtual void					subBlockRemoved(AbstractBlock &subBlock) = 0;
		virtual void					uberBlockAdded(AbstractBlock &uberBlock) = 0;
		virtual void					uberBlockRemoved(AbstractBlock &uberBlock) = 0;

	protected:

		AbstractBlock					&m_Owner;

	};

}