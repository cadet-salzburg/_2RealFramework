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

#include "bundle/_2RealParameterHandle.h"
#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"
#include "engine/_2RealTimestampedData.h"
#include "helpers/_2RealPoco.h"

namespace _2Real
{

	// base class for inlets & outlets
	// will also be base class for new setup param
	// 'parameter' is probably stupid name, what was i thinking

	class AbstractUberBlock;

	class Parameter
	{

	protected:

		Parameter( std::shared_ptr< const CustomType > initializer );
		virtual ~Parameter() {}

		// data = buffer
		void									synchronize();
		// this is for reading only ( app, inlet queues ) -> const
		std::shared_ptr< const CustomType >		getData() const;
		std::shared_ptr< CustomType >			getWriteableData();

		mutable Poco::FastMutex			m_DataAccess;
		std::shared_ptr< const CustomType >	m_Data;
		std::shared_ptr< CustomType >		m_DataBuffer;

	};

	class InputParameter : private NonCopyable< InputParameter >, private Identifiable< InputParameter >, private Handleable< InputParameter, bundle::ParameterHandle >
	{
		
	public:

		InputParameter( AbstractUberBlock &owningBlock, std::string const& name, std::shared_ptr< const CustomType > initialValue );

		using Handleable< InputParameter, bundle::ParameterHandle >::getHandle;
		using Handleable< InputParameter, bundle::ParameterHandle >::registerHandle;
		using Handleable< InputParameter, bundle::ParameterHandle >::unregisterHandle;

		using Identifiable< InputParameter >::getFullName;
		using Identifiable< InputParameter >::getName;

		std::shared_ptr< const CustomType >		getData() const;
		std::shared_ptr< const CustomType >		getDataThreadsafe() const;
		void									setData( std::shared_ptr< const CustomType > data );
		bool									hasChanged() const;
		void									synchronize();

		AbstractUberBlock&		getOwningUberBlock();

	private:

		AbstractUberBlock						&m_OwningUberBlock;
		mutable Poco::FastMutex					m_DataAccess;
		std::shared_ptr< const CustomType >		m_Data;
		std::shared_ptr< const CustomType >		m_DataBuffer;
		std::shared_ptr< const CustomType >		m_LastData;

	};

}