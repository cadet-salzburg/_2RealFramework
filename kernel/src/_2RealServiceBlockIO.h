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

#include "_2RealAbstractIOManager.h"

#include "Poco/Mutex.h"
#include "Poco/Timestamp.h"

#include <map>
#include <string>

namespace _2Real
{

	class BlockData;
	class ParameterData;
	class EngineData;
	class TimestampedData;
	class InletHandle;
	class OutletHandle;
	class AbstractBlock;
	class BufferPolicy;
	class Inlet;
	class Outlet;
	class SetupParameter;
	class UpdatePolicyImpl;

	typedef std::map< std::string, Inlet * >		InletMap;
	typedef std::map< std::string, Outlet * >		OutletMap;
	typedef std::map< std::string, SetupParameter * >	ParamMap;

	class ServiceIO : public AbstractIOManager
	{

	public:

		ServiceIO(AbstractBlock &owner);
		~ServiceIO();

		void						initFrom( BlockData const& meta, Poco::Timestamp const& time, UpdatePolicyImpl const& policy );

		void						clear();
		//void						addParam(ParameterData const& meta);
		//void						addInlet(ParameterData const& meta);
		//void						addOutlet(ParameterData const& meta, Poco::Timestamp const& timestamp);
		void						registerToNewData(std::string const& outName, OutletCallback callback, void *userData );
		void						unregisterFromNewData(std::string const& outName, OutletCallback callback, void *userData );
		void						registerToNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler);
		void						unregisterFromNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler);
		void						registerToNewData(OutputCallback callback, void *userData );
		void						unregisterFromNewData(OutputCallback callback, void *userData );
		void						registerToNewData(AbstractOutputCallbackHandler &handler);
		void						unregisterFromNewData(AbstractOutputCallbackHandler &handler);
		const EngineData			getValue(std::string const& paramName) const;
		std::string const&			getKey(std::string const& paramName) const;
		void						setValue(std::string const& paramName, TimestampedData const& value);
		void						insertValue(std::string const& paramName, TimestampedData &value);

		void						linkWith(std::string const& inlet, AbstractBlock &out, std::string const& outlet);
		
		InletHandle					createInletHandle(std::string const& name);
		OutletHandle				createOutletHandle(std::string const& name);

		void						syncInlets();
		void						updateOutlets();
		void						updateInlets();

		InletMap const&				getInlets() const;
		OutletMap const&			getOutlets() const;

		void						subBlockAdded(AbstractBlock &subBlock);
		void						subBlockRemoved(AbstractBlock &subBlock);
		void						uberBlockAdded(AbstractBlock &uberBlock);
		void						uberBlockRemoved(AbstractBlock &uberBlock);

	private:

		mutable Poco::FastMutex		m_InletAccess;
		mutable Poco::FastMutex		m_OutletAccess;
		mutable Poco::FastMutex		m_ParamAccess;

		InletMap					m_Inlets;
		OutletMap					m_Outlets;
		ParamMap					m_Params;

		OutletFunctionCallbacks		m_OutletFunctionCallbacks;
		OutletCallbackHandlers		m_OutletCallbackHandlers;
		OutputFunctionCallbacks		m_OutputFunctionCallbacks;
		OutputCallbackHandlers		m_OutputCallbackHandlers;

		std::list< OutputData >		m_OutputData;

	};

}