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

#include "_2RealBlock.h"
#include "_2RealDisabledBlocks.h"
#include "_2RealServiceIO.h"
#include "_2RealServiceStates.h"
#include "_2RealServiceData.h"

namespace _2Real
{

	class SystemImpl;
	class UpdatePolicyImpl;

	class ServiceBlock : public Block< ServiceIO, DisabledBlocks, DisabledBlocks, ServiceStates/*, ServiceUpdates */ >
	{

	public:

		ServiceBlock(ServiceData const& data, SystemImpl &owner, UpdatePolicyImpl const& triggers);
		~ServiceBlock();

		void			setUpService();
		void			executeService();
		void			shutDownService();
		InputHandle		createInputHandle(std::string const& inletName);
		OutputHandle	createOutputHandle(std::string const& outletName);

	private:

		IService	*m_Service;

	};

}