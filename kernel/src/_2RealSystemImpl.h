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
#include "_2RealOwnedBlocks.h"
#include "_2RealDisabledIO.h"
#include "_2RealDisabledStates.h"
#include "_2RealExceptionHandler.h"

#include "Poco/Path.h"
#include "Poco/Timer.h"

namespace _2Real
{

	class EngineImpl;
	class PluginPool;
	class EngineData;
	class UpdatePolicy;
	class IOutputListener;

	class SystemImpl : public Block< DisabledIO, DisabledBlocks, OwnedAndUnordered, DisabledStates/*, SystemUpdates */ >
	{

	public:

		SystemImpl(std::string const& name);
		~SystemImpl();

		void						clear();

		const long					getElapsedTime() const;
		Poco::Timestamp	const&		getTimestamp() const;

		const Identifier			createServiceBlock(Identifier const& pluginId, std::string const& serviceName, UpdatePolicy const& triggers);
		const Identifier			createSyncBlock(std::list< Identifier > const& blockIds);
		//const Identifier			createSyncBlock(std::list< Identifier > const& blockIds, std::list< Identifier > const& readyIds, std::list< Identifier > const& finishedIds);

		void						handleException(AbstractBlock &subBlock, Exception &exception);
		void						registerToNewData(Identifier const& id, std::string const& outlet, DataCallback callback, void *userData);
		void						unregisterFromNewData(Identifier const& id, std::string const& outlet, DataCallback callback, void *userData);
		void						registerToNewData(Identifier const& id, std::string const& outlet, AbstractDataCallbackHandler &handler );
		void						unregisterFromNewData(Identifier const& id, std::string const& outlet, AbstractDataCallbackHandler &handler );

		void						setUp(Identifier const& blockId);

		const EngineData			getValue(Identifier const& ownerId, std::string const& paramName) const;
		std::string const&			getKey(Identifier const& ownerId, std::string const& paramName) const;
		void						setValue(Identifier const& ownerId, std::string const& paramName, EngineData const& value);
		void						insertValue(Identifier const& ownerId, std::string const& paramName, EngineData const& value);
		void						link(Identifier const& ownerIn, std::string const& in, Identifier const& ownerOut, std::string const& out);

	private:

		EngineImpl					&m_Engine;
		PluginPool					&m_PluginPool;
		Poco::Timestamp				m_Timestamp;
		ExceptionHandler			m_ExceptionHandler;
		ExceptionFunctionCallbacks	m_ExceptionCallbacks;

	};

}