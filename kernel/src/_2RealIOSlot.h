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

#include "_2RealParameter.h"

namespace _2Real
{

	/**
	*	represents an input or output slot of a service
	*/

	class Service;

	class IOSlot : public Parameter
	{

	public:

		IOSlot(Identifier const& id, Service &service, std::string const& type, std::string const& keyword);
		virtual ~IOSlot();

		Service &			service();
		Service const&		service() const;

	protected:

		Service				&m_Service;

	};

	inline Service const& IOSlot::service() const
	{
		return m_Service;
	}

	inline Service & IOSlot::service()
	{
		return m_Service;
	}

}