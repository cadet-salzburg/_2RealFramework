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

	class ServiceMetadata;
	class ParameterMetadata;
	class IOutputListener;
	class EngineData;
	class Data;
	class InputHandle;
	class OutputHandle;
	class AbstractBlock;
	class BufferPolicy;
	class InputSlot;
	class OutputSlot;
	class SetupParameter;

	typedef std::map< std::string, InputSlot * >		InletMap;
	typedef std::map< std::string, OutputSlot * >		OutletMap;
	typedef std::map< std::string, SetupParameter * >	ParamMap;

	typedef void (*DataCallback)(Data &data);

	class ServiceIO : public AbstractIOManager
	{

	public:

		ServiceIO(AbstractBlock &owner);
		~ServiceIO();

		void						initFrom(ServiceMetadata &meta, Poco::Timestamp const& time);

		void						clear();
		//void						addParam(ParameterMetadata const& meta);
		//void						addInlet(ParameterMetadata const& meta);
		//void						addOutlet(ParameterMetadata const& meta, Poco::Timestamp const& timestamp);
		void						registerToNewData(std::string const& outName, DataCallback callback);
		void						unregisterFromNewData(std::string const& outName, DataCallback callback);
		void						registerToNewData(std::string const& outName, IOutputListener &listener);
		void						unregisterFromNewData(std::string const& outName, IOutputListener &listener);
		const EngineData			getValue(std::string const& paramName) const;
		std::string const&			getKey(std::string const& paramName) const;
		void						setValue(std::string const& paramName, Data const& value);
		void						insertValue(std::string const& paramName, Data const& value);

		void						linkWith(std::string const& inlet, AbstractBlock &out, std::string const& outlet);
		
		InputHandle					createInputHandle(std::string const& name);
		OutputHandle				createOutputHandle(std::string const& name);

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

		BufferPolicy				*m_Policy;

	};

}