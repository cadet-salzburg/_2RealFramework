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

#include "engine/_2RealAbstractIOManager.h"
#include "app/_2RealCallbacks.h"
#include "bundle/_2RealBlockHandle.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class FunctionBlockStateManager;
	class FunctionBlockUpdateManager;
	class IOMetadata;
	class FunctionBlock;

	class FunctionBlockIOManager : private AbstractIOManager
	{

		friend class FunctionBlock;

	public:

		FunctionBlockIOManager( EngineImpl *engine, FunctionBlock *owner );
		~FunctionBlockIOManager();

		std::string const&							getFullHumanReadableName() const;
		std::string const&							getHumanReadableName() const;
		//std::string const&							getCode() const;

		void										clear();

		void										registerToNewData( app::BlockCallback &cb );
		void										unregisterFromNewData( app::BlockCallback &cb );

		void										addInlet( std::shared_ptr< const IOMetadata > );
		void										addOutlet( std::shared_ptr< const IOMetadata > );
		void										addParameter( std::shared_ptr< const IOMetadata > );

		std::shared_ptr< AbstractInletIO >			getInlet( std::string const& );
		std::shared_ptr< const AbstractInletIO >	getInlet( std::string const& ) const;
		std::shared_ptr< OutletIO >					getOutlet( std::string const& );
		std::shared_ptr< const OutletIO >			getOutlet( std::string const& ) const;
		std::shared_ptr< AbstractInletIO >			getParameter( std::string const& );
		std::shared_ptr< const AbstractInletIO >	getParameter( std::string const& ) const;

		InletVector &								getAllInlets();
		OutletVector &								getAllOutlets();
		ParameterVector &							getAllParameters();
		InletVector const&							getAllInlets() const;
		OutletVector const&							getAllOutlets() const;
		ParameterVector const&						getAllParameters() const;

		void										updateInletData();
		void										updateParameterData();
		void										updateOutletData();

	private:

		EngineImpl						*const mEngineImpl;
		FunctionBlock					*const mOwningBlock;

		std::weak_ptr< FunctionBlockUpdateManager >		mUpdateManager;

		mutable Poco::FastMutex			m_InletAccess;
		mutable Poco::FastMutex			m_OutletAccess;
		mutable Poco::FastMutex			m_ParameterAccess;

		InletVector						mInlets;
		OutletVector					mOutlets;
		ParameterVector					mParameters;

		Poco::FastMutex																m_IOAccess;
		CallbackEvent< std::vector< std::shared_ptr< const CustomType > > >			m_AppEvent;

	};

}