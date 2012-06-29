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

		void							registerToNewData( std::string const& outName, app::OutletDataCallback callback, void *userData );
		void							unregisterFromNewData( std::string const& outName, app::OutletDataCallback callback, void *userData );
		void							registerToNewData( std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler );
		void							unregisterFromNewData( std::string const& outlet, app::AbstractOutletDataCallbackHandler &handler );
		void							registerToNewData( app::BlockDataCallback callback, void *userData );
		void							unregisterFromNewData( app::BlockDataCallback callback, void *userData );
		void							registerToNewData( app::AbstractBlockDataCallbackHandler &handler );
		void							unregisterFromNewData( app::AbstractBlockDataCallbackHandler &handler );

		Inlet &							getInlet( std::string const& name );
		Outlet &						getOutlet( std::string const& name );
		Parameter &						getParameter( std::string const& name );

	};

}