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

#include "_2RealTypedefs.h"

#include <list>
#include <string>

#include "Poco/Mutex.h"

namespace _2Real
{

	class OutputListener
	{

		friend class Configuration;

	public:

		/**
		*
		*/
		void listenerFunction(DataPtr &_data);

	private:

		/**
		*
		*/
		OutputListener(std::string const& _name, Variable const& _fwName, unsigned long const& _time);

		/**
		*
		*/
		OutputListener(OutputListener const& _src);

		/**
		*
		*/
		OutputListener& operator=(OutputListener const& _src);

		/**
		*
		*/
		~OutputListener();

		/**
		*
		*/
		const bool hasReceivedData(unsigned long const& _compare) const;

		/**
		*
		*/
		AnyPtr getData();

		/**
		*
		*/
		typedef std::list< AnyPtr >		AnyQueue;

		/**
		*
		*/
		AnyQueue						m_AnyQueue;

		/**
		*
		*/
		std::string						m_ListenerName;

		/**
		*
		*/
		Variable						m_FrameworkName;

		/**
		*
		*/
		unsigned long					m_tDataReceived;

		/**
		*
		*/
		Poco::Mutex						m_Mutex;

	};

}