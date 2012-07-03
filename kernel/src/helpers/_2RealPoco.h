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

#include "Poco/Timestamp.h"
#include "Poco/Timer.h"
#include "Poco/Path.h"
#include "Poco/SharedLibrary.h"
#include "Poco/Mutex.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Event.h"
#include "Poco/ThreadLocal.h"
//#include "Poco/Delegate.h"
//#include "Poco/BasicEvent.h"

//namespace _2Real
//{
	//typedef Poco::Timestamp							Timestamp;
	//typedef Poco::Timer								Timer;
	//typedef Poco::Path								Path;
	//typedef Poco::SharedLibrary						SharedLibrary;
	//typedef Poco::FastMutex							FastMutex;
	//typedef Poco::Event								Event;
	//typedef Poco::ScopedLock< Poco::FastMutex >		ScopedLock;
	//typedef Poco::Thread								Thread;
	//typedef Poco::Runnable							Runnable;
//}