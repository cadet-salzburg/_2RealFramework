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

#include "_2RealFramework.h"

#include "Poco/Mutex.h"

/*
	entry point into _2Real framework
*/

namespace _2Real
{
	class _2RealContext
	{

	public:

		~_2RealContext();
		static _2RealContextPtr instance();

		void config();
		bool update();
		void start();
		void stop();

	private:

		_2RealContext();

		static _2RealContextPtr		s_ContextPtr;
		static bool					s_bIsInstanced;
		static Poco::Mutex			s_Mutex;

		Poco::Mutex					m_Lock;
		_2RealContextPrivate*		m_PrivateContext;

	};
}