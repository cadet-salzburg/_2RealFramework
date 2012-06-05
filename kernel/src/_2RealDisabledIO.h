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

namespace _2Real
{

	class DisabledIO : public AbstractIOManager
	{

	public:

		DisabledIO(AbstractBlock &owner);
		~DisabledIO();

		void						clear();
		//void						addParam(ParameterData const& meta);
		//void						addInlet(ParameterData const& meta);
		//void						addOutlet(ParameterData const& meta, Poco::Timestamp const& timestamp);
		void						registerToNewData( std::string const& outName, DataCallback callback, void *userData );
		void						unregisterFromNewData( std::string const& outName, DataCallback callback, void *userData );
		void						registerToNewData(std::string const& outlet, AbstractDataCallbackHandler &handler);
		void						unregisterFromNewData(std::string const& outlet, AbstractDataCallbackHandler &handler);
		const EngineData			getValue(std::string const& paramName) const;
		std::string const&			getKey(std::string const& paramName) const;
		void						setValue(std::string const& paramName, Data const& value);
		void						insertValue(std::string const& paramName, Data &value);
		void						linkWith(std::string const& inlet, AbstractBlock &out, std::string const& outlet);

		InletMap const&				getInlets() const;
		OutletMap const&			getOutlets() const;

		void						subBlockAdded(AbstractBlock &subBlock);
		void						subBlockRemoved(AbstractBlock &subBlock);
		void						uberBlockAdded(AbstractBlock &uberBlock);
		void						uberBlockRemoved(AbstractBlock &uberBlock);

	};

}