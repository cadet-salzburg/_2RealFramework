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
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"
#include "_2RealEngineData.h"

#include "Poco/Mutex.h"

#include <string>

namespace _2Real
{

	class Data;
	class AbstractUpdateManager;
	class AbstractStateManager;
	class AbstractBlockManager;
	class AbstractIOManager;

	class InputSlot;
	class OutputSlot;
	class SetupParameter;

	typedef std::map< std::string, InputSlot * >		InletMap;
	typedef std::map< std::string, OutputSlot * >		OutletMap;
	typedef std::map< std::string, SetupParameter * >	ParamMap;

	typedef void (*DataCallback)(Data &data);

	class AbstractBlock : public Entity
	{

	public:

		AbstractBlock(std::string const& name, AbstractBlock *superBlock);
		virtual ~AbstractBlock();

		void									lockSuperBlock();
		void									unlockSuperBlock();
		AbstractBlock &							getSuperBlock();
		AbstractBlock const&					getSuperBlock() const;
		void									setSuperBlock(AbstractBlock &superBlock);

		virtual void							addSubBlock(AbstractBlock &subBlock) = 0;
		virtual void							removeSubBlock(AbstractBlock &subBlock) = 0;
		virtual void							addUberBlock(AbstractBlock &uberBlock) = 0;
		virtual void							removeUberBlock(AbstractBlock &uberBlock) = 0;
		//virtual std::list< Identifier >			getCurrentSubBlocks() const = 0;
		//virtual std::list< Identifier >			getCurrentUberBlocks() const = 0;

		virtual void							registerToNewData(std::string const& outlet, DataCallback callback) = 0;
		virtual void							unregisterFromNewData(std::string const& outlet, DataCallback callback) = 0;
		virtual const EngineData				getValue(std::string const& paramName) const = 0;
		virtual std::string const&				getKey(std::string const& paramName) const = 0;
		virtual void							setValue(std::string const& paramName, Data const& value) = 0;
		virtual void							insertValue(std::string const& paramName, Data const& value) = 0;
		virtual void							linkWith(std::string const& nameIn, AbstractBlock &out, std::string const& nameOut) = 0;

		virtual void							setUp() = 0;
		virtual void							prepareForShutDown() = 0;
		virtual const bool						shutDown() = 0;

		virtual AbstractStateManager &			getStateManager() = 0;
		//virtual AbstractUpdateManager &			getTriggerManager() = 0;
		virtual AbstractBlockManager &			getSubBlockManager() = 0;
		virtual AbstractBlockManager &			getUberBlockManager() = 0;
		virtual AbstractIOManager &				getIOManager() = 0;

		virtual AbstractStateManager const&		getStateManager() const = 0;
		//virtual AbstractUpdateManager const&	getTriggerManager() const = 0;
		virtual AbstractBlockManager const&		getSubBlockManager() const = 0;
		virtual AbstractBlockManager const &	getUberBlockManager() const = 0;
		virtual AbstractIOManager const&		getIOManager() const = 0;

	protected:

		mutable Poco::FastMutex					m_SuperBlockAccess;
		AbstractBlock							*m_SuperBlock;

	};

}