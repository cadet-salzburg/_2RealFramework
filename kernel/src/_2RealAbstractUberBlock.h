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

#include "_2RealEntity.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"
#include "_2RealEngineData.h"
#include "_2RealCallbacks.h"

#include "Poco/Mutex.h"

#include <map>
#include <string>

namespace _2Real
{

	class AbstractUpdateManager;
	class AbstractStateManager;
	class AbstractBlockManager;
	class AbstractIOManager;

	class Inlet;
	class Outlet;
	class SetupParameter;
	class TimestampedData;
	class UpdatePolicyImpl;

	typedef std::map< std::string, Inlet * >		InletMap;
	typedef std::map< std::string, Outlet * >		OutletMap;
	typedef std::map< std::string, SetupParameter * >	ParamMap;

	class AbstractBlock : public Entity
	{

	public:

		AbstractBlock(std::string const& name, AbstractBlock *superBlock);
		virtual ~AbstractBlock();

		BlockIdentifier							getIdentifier() const;

		void									lockSuperBlock();
		void									unlockSuperBlock();
		AbstractBlock &							getSuperBlock();
		AbstractBlock const&					getSuperBlock() const;
		void									setSuperBlock(AbstractBlock &superBlock);

		virtual void							addSubBlock( AbstractBlock &subBlock ) = 0;
		virtual void							removeSubBlock( AbstractBlock &subBlock ) = 0;
		virtual void							addUberBlock( AbstractBlock &uberBlock ) = 0;
		virtual void							removeUberBlock( AbstractBlock &uberBlock ) = 0;
		//virtual std::list< Identifier >			getCurrentSubBlocks() const = 0;
		//virtual std::list< Identifier >			getCurrentUberBlocks() const = 0;

		virtual void							registerToNewData(std::string const& outlet, OutletCallback callback, void *userData) = 0;
		virtual void							unregisterFromNewData(std::string const& outlet, OutletCallback callback, void *userData) = 0;
		virtual void							registerToNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler) = 0;
		virtual void							unregisterFromNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler) = 0;
		virtual void							registerToNewData(OutputCallback callback, void *userData) = 0;
		virtual void							unregisterFromNewData(OutputCallback callback, void *userData) = 0;
		virtual void							registerToNewData(AbstractOutputCallbackHandler &handler) = 0;
		virtual void							unregisterFromNewData(AbstractOutputCallbackHandler &handler) = 0;

		virtual const EngineData				getValue(std::string const& paramName) const = 0;
		virtual std::string const&				getKey(std::string const& paramName) const = 0;
		virtual void							setValue(std::string const& paramName, TimestampedData const& value) = 0;
		virtual void							insertValue(std::string const& paramName, TimestampedData &value) = 0;
		virtual void							linkWith(std::string const& nameIn, AbstractBlock &out, std::string const& nameOut) = 0;

		virtual void							setUp() = 0;
		virtual void							start() = 0;
		virtual void							stop( const bool blocking, const long timeout ) = 0;
		virtual void							prepareForShutDown() = 0;
		virtual bool							shutDown( const long timeout ) = 0;
		virtual void							setUpdatePolicy( UpdatePolicyImpl const& policy ) = 0;

		virtual AbstractStateManager &			getStateManager() = 0;
		virtual AbstractBlockManager &			getSubBlockManager() = 0;
		virtual AbstractBlockManager &			getUberBlockManager() = 0;
		virtual AbstractIOManager &				getIOManager() = 0;

		virtual AbstractStateManager const&		getStateManager() const = 0;
		virtual AbstractBlockManager const&		getSubBlockManager() const = 0;
		virtual AbstractBlockManager const &	getUberBlockManager() const = 0;
		virtual AbstractIOManager const&		getIOManager() const = 0;

	protected:

		mutable Poco::FastMutex					m_SuperBlockAccess;
		AbstractBlock							*m_SuperBlock;

	};

}