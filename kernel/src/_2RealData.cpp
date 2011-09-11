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

#include "_2RealData.h"
#include "_2RealDataImpl.h"

#include <iostream>

namespace _2Real
{

	Data::Data(Poco::SharedPtr< DataPacket > _data, Identifier const& _id) :
		m_Data(_data),
		m_ID(_id)
	{
	}

	Poco::SharedPtr< Poco::Any > Data::get(unsigned int const& _id)
	{
		return m_Data->getAny(_id);
	}

	const Identifier Data::id() const
	{
		return m_ID;
	}
}