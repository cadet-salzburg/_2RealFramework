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

#include "_2RealUberBlock.h"
#include "_2RealDisabledBlocks.h"
#include "_2RealOwnedBlocks.h"
#include "_2RealDisabledIO.h"
#include "_2RealDisabledStates.h"

namespace _2Real
{

	class EngineImpl;
	class BundleManager;
	class EngineData;
	class UpdatePolicy;
	class IOutputListener;
	class FunctionBlock;
	class BlockData;
	class BundleIdentifier;
	class BlockIdentifier;

	class SystemImpl : public UberBlock< DisabledIO, DisabledBlocks, OwnedAndUnordered, SystemStates >
	{

	public:

		SystemImpl( BlockIdentifier const& id );
		~SystemImpl();

		void						clear();

		const BlockIdentifier		createFunctionBlock( BundleIdentifier const& pluginId, std::string const& serviceName );

		void						handleException( AbstractBlock &subBlock, Exception &exception );

		void						registerToNewData( BlockIdentifier const& id, std::string const& outlet, OutletCallback callback, void *userData );
		void						unregisterFromNewData( BlockIdentifier const& id, std::string const& outlet, OutletCallback callback, void *userData );
		void						registerToNewData( BlockIdentifier const& id, std::string const& outlet, AbstractOutletCallbackHandler &handler );
		void						unregisterFromNewData( BlockIdentifier const& id, std::string const& outlet, AbstractOutletCallbackHandler &handler );

		void						registerToNewData( BlockIdentifier const& id, OutputCallback callback, void *userData );
		void						unregisterFromNewData( BlockIdentifier const& id, OutputCallback callback, void *userData );
		void						registerToNewData( BlockIdentifier const& id, AbstractOutputCallbackHandler &handler );
		void						unregisterFromNewData( BlockIdentifier const& id, AbstractOutputCallbackHandler &handler );

		void						registerToException( ExceptionCallback callback, void *userData );
		void						unregisterFromException( ExceptionCallback callback, void *userData );
		void						registerToException( AbstractExceptionCallbackHandler &handler );
		void						unregisterFromException( AbstractExceptionCallbackHandler &handler );

		void						setUp( BlockIdentifier const& blockId );
		void						start( BlockIdentifier const& blockId );
		void						stop( BlockIdentifier const& blockId, const long timeout );
		void						singleStep( BlockIdentifier const& blockId );
		void						destroy( BlockIdentifier const& blockId );
		void						setUpdatePolicy( BlockIdentifier const& blockId, UpdatePolicy const& policy );

		const EngineData			getValue(BlockIdentifier const& ownerId, std::string const& paramName) const;
		std::string const&			getKey(BlockIdentifier const& ownerId, std::string const& paramName) const;
		void						setValue(BlockIdentifier const& ownerId, std::string const& paramName, EngineData const& value);
		void						insertValue(BlockIdentifier const& ownerId, std::string const& paramName, EngineData const& value);
		void						link(BlockIdentifier const& ownerIn, std::string const& in, BlockIdentifier const& ownerOut, std::string const& out);

	private:

		EngineImpl					&m_Engine;
		BundleManager				&m_BundleManager;

		// handles exception callbacks
		mutable Poco::FastMutex		m_ExceptionAccess;
		ExceptionFunctionCallbacks	m_ExceptionCallbacks;
		ExceptionCallbackHandlers	m_ExceptionCallbackHandlers;

	};

}