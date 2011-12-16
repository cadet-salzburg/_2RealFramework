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

#include "_2RealRunnableStates.h"
#include "_2RealRunnableManager.h"
#include "_2RealRunnable.h"
#include "_2RealException.h"
#include "_2RealPooledThread.h"

namespace _2Real
{

	RunnableState::RunnableState(RunnableManager const& manager, std::string const& description) :
		m_Description(description),
		m_Manager(manager)
	{
	}

	RunnableState::~RunnableState()
	{
	}

	RunnableStateCreated::RunnableStateCreated(RunnableManager const& manager) :
		RunnableState(manager, "this runnable was just created and should be set up now")
	{
	}

	RunnableState & RunnableStateCreated::start(Runnable &runnable, PooledThread &thread, bool runOnce)
	{
		std::ostringstream msg;
		msg << "runnable " << runnable.name() << " can't be started, needs to be set up first";
		throw StartException(msg.str());
	}

	RunnableState & RunnableStateCreated::update(Runnable &runnable)
	{
		std::ostringstream msg;
		msg << "runnable " << runnable.name() << " can't be updated, needs to be set up first";
		throw StartException(msg.str());
	}

	RunnableState & RunnableStateCreated::stop(Runnable &runnable)
	{
		//does not have any effect here, stop() has the runnable go back to 'set up' anyway
		return *this;
	}

	RunnableState & RunnableStateCreated::setup(Runnable &runnable)
	{
		try
		{
			runnable.setup();
		}
		catch (SetupFailedException &e)
		{
			return *this;
		}
		catch (ServiceException &e)
		{
			return m_Manager.getState("halted");
		}
		catch (...)
		{
			return m_Manager.getState("halted");
		}
	}

	RunnableState & RunnableStateCreated::shutdown(Runnable &runnable)
	{
		try
		{
			runnable.shutdown();
		}
		catch (...)
		{
			return m_Manager.getState("halted");
		}

		return m_Manager.getState("shut down");
	}

	RunnableState & RunnableStateCreated::handleException(Runnable &runnable)
	{
		return m_Manager.getState("halted");
	}

	RunnableStateSetUp::RunnableStateSetUp(RunnableManager const& manager) :
		RunnableState(manager, "this runnable is set up and ready to run")
	{
	}

	RunnableState & RunnableStateSetUp::start(Runnable &runnable, PooledThread &thread, bool runOnce)
	{
		//runnable.start(runOnce);
		//thread.start(Poco::Thread::PRIO_NORMAL, runnable);
		return m_Manager.getState("started");
	}

	RunnableState & RunnableStateSetUp::update(Runnable &runnable)
	{
		//update is blocking
		runnable.update();
		return *this;
	}

	RunnableState & RunnableStateSetUp::stop(Runnable &runnable)
	{
		//does not have any effect here
		return *this;
	}

	RunnableState & RunnableStateSetUp::setup(Runnable &runnable)
	{
		//just call setup again
		runnable.setup();
		return *this;
	}

	RunnableState & RunnableStateSetUp::shutdown(Runnable &runnable)
	{
		runnable.shutdown();
		return m_Manager.getState("shut down");
	}

	RunnableState & RunnableStateSetUp::handleException(Runnable &runnable)
	{
		//is there anything else to do?
		return m_Manager.getState("halted");
	}

	RunnableStateStarted::RunnableStateStarted(RunnableManager const& manager) :
		RunnableState(manager, "this runnable is running currently")
	{
	}

	RunnableState & RunnableStateStarted::start(Runnable &runnable, PooledThread &thread, bool runOnce)
	{
		return *this;
	}

	RunnableState & RunnableStateStarted::update(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateStarted::stop(Runnable &runnable)
	{
		runnable.stop();
		return m_Manager.getState("set up");
	}

	RunnableState & RunnableStateStarted::setup(Runnable &runnable)
	{
		//this is ignored, runnable would need to be stopped first
		return *this;
	}

	RunnableState & RunnableStateStarted::shutdown(Runnable &runnable)
	{
		runnable.stop();
		runnable.shutdown();
		return m_Manager.getState("shut down");
	}

	RunnableState & RunnableStateStarted::handleException(Runnable &runnable)
	{
		//is there anything else to do?
		return m_Manager.getState("halted");
	}

	RunnableStateHalted::RunnableStateHalted(RunnableManager const& manager) :
		RunnableState(manager, "an exception occured in the runnable; is is currently halted")
	{
	}

	RunnableState & RunnableStateHalted::start(Runnable &runnable, PooledThread &thread, bool runOnce)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::update(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::stop(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::setup(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::shutdown(Runnable &runnable)
	{
		runnable.shutdown();
		return m_Manager.getState("shut down");
	}

	RunnableState & RunnableStateHalted::handleException(Runnable &runnable)
	{
		return *this;
	}

	RunnableStateShutDown::RunnableStateShutDown(RunnableManager const& manager) :
		RunnableState(manager, "this runnable was shut down and will be destroyed soon")
	{
	}

	RunnableState & RunnableStateShutDown::start(Runnable &runnable, PooledThread &thread, bool runOnce)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::update(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::stop(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::setup(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::shutdown(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::handleException(Runnable &runnable)
	{
		return *this;
	}

}