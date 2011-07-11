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

#include "_2RealContext.h"

namespace _2Real
{
	bool _2RealContext::s_bIsInstanced = false;
	_2RealContextPtr _2RealContext::s_ContextPtr;
	Poco::Mutex _2RealContext::s_Mutex;

	_2RealContextPtr _2RealContext::instance()
	{
		Poco::ScopedLock<Poco::Mutex> lock(s_Mutex);
		if(!s_bIsInstanced)
		{
			s_ContextPtr = _2RealContextPtr(new _2RealContext());
			s_bIsInstanced = true;
			return s_ContextPtr;
		}
		else
		{
			return s_ContextPtr;
		}
	}

	_2RealContext::_2RealContext()
	{
	}

	_2RealContext::~_2RealContext()
	{
	}

	/*
	void _2RealContext::config()
	{
	}

	void _2RealContext::update()
	{
	}

	void _2RealContext::start()
	{
	}

	void _2RealContext::stop()
	{
	}
	*/
}