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

		// sets the buffer - not sure why this is needed, maybe setup p?
		//	void						setData( std::shared_ptr< CustomType > data );

		// data = buffer
		void									synchronize();
		// this is for reading only ( app, inlet queues ) -> const
		std::shared_ptr< const CustomType >		getData() const;
		CustomType &							getWriteableData();

		mutable Poco::FastMutex			m_DataAccess;
		std::shared_ptr< CustomType >	m_Data;
		std::shared_ptr< CustomType >	m_DataBuffer;

	};

}