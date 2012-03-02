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

#include "_2RealExceptionHandler.h"
#include "_2RealRunnableError.h"
#include "_2RealIExceptionListener.h"

#include "Poco/Delegate.h"

namespace _2Real
{

	ExceptionHandler::ExceptionHandler(Identifier const& system) :
		m_System(system)
	{
	}

	void ExceptionHandler::registerExceptionCallback(ExceptionCallback callback)
	{
		m_Event += Poco::delegate(callback);
	}

	void ExceptionHandler::unregisterExceptionCallback(ExceptionCallback callback)
	{
		m_Event -= Poco::delegate(callback);
	}

	void ExceptionHandler::registerExceptionListener(IExceptionListener &listener)
	{
		m_Event += Poco::delegate(&listener, &IExceptionListener::receiveException);
	}

	void ExceptionHandler::unregisterExceptionListener(IExceptionListener &listener)
	{
		m_Event -= Poco::delegate(&listener, &IExceptionListener::receiveException);
	}

	void ExceptionHandler::handleException(Exception const& exception, Identifier const& sender)
	{
		RunnableError e(exception, sender, m_System);
		m_Event.notifyAsync(this, e);
	}
}