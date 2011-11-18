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
#include "_2RealSharedAny.h"

namespace _2Real
{

	/**
	*	class for setup parameters: service-setup & plugin-setup
	*/

	class SetupParameter : public Parameter
	{

	public:

		SetupParameter(Id *const id, std::string const& type, std::string const& key);
		virtual ~SetupParameter();

		/**
		*	sets parameter data
		*/
		void set(SharedAny const& data)
		{
			m_Data = data;
			//m_IsInitialized = true;
		}

		/**
		*	returns parameter data
		*/
		SharedAny const& get() const
		{
			return m_Data;
		}

	protected:

		SharedAny				m_Data;

	};

}