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

	class DisabledIOManager : public AbstractIOManager
	{

	public:

		DisabledIOManager(AbstractUberBlock &owner);
		~DisabledIOManager();

		void							clear();
		void							registerToNewData( std::string const& outName, OutletCallback callback, void *userData );
		void							unregisterFromNewData( std::string const& outName, OutletCallback callback, void *userData );
		void							registerToNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler);
		void							unregisterFromNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler);
		void							registerToNewData(OutputCallback callback, void *userData );
		void							unregisterFromNewData(OutputCallback callback, void *userData );
		void							registerToNewData(AbstractOutputCallbackHandler &handler);
		void							unregisterFromNewData(AbstractOutputCallbackHandler &handler);

		EngineData const&				getValue(std::string const& paramName) const;
		std::string const&				getTypename( std::string const& paramName ) const;
		std::string const&				getLongTypename( std::string const& paramName ) const;
		void							setValue(std::string const& paramName, TimestampedData const& value);

		Inlet const&					getInlet( std::string const& name ) const;
		Outlet const&					getOutlet( std::string const& name ) const;
		SetupParameter const&			getSetupParameter( std::string const& name ) const;

		Inlet &							getInlet( std::string const& name );
		Outlet &						getOutlet( std::string const& name );
		SetupParameter &				getSetupParameter( std::string const& name );

		void							addInlet( ParameterData const& data );
		void							addOutlet( ParameterData const& data );
		void							addSetupParameter( ParameterData const& data );

	};

}