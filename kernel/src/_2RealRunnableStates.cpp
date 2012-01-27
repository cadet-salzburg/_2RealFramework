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

#include <iostream>

namespace _2Real
{

	RunnableState::RunnableState(RunnableManager const& manager, std::string const& name, std::string const& description) :
		m_Description(description),
		m_Name(name),
		m_Manager(manager)
	{
	}

	RunnableState::~RunnableState()
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////

	RunnableStateCreated::RunnableStateCreated(RunnableManager const& manager) :
		RunnableState(manager, "created", "this runnable was just created and should be set up now")
	{
	}

	RunnableState & RunnableStateCreated::setup(Runnable &runnable)
	{
		try
		{
			runnable.setup();
		}
		catch (SetupException &e)
		{
			return *this;
		}
		catch (ServiceException &e)
		{
			return m_Manager.changeState("halted");
		}
		catch (...)
		{
			return m_Manager.changeState("halted");
		}

		return m_Manager.changeState("set up");
	}

	RunnableState & RunnableStateCreated::start(Runnable &runnable, PooledThread &thread)
	{
		std::ostringstream msg;
		msg << "runnable " << runnable.name() << " can't be started, needs to be set up first";
		throw StartException(msg.str());
	}

	RunnableState & RunnableStateCreated::update(Runnable &runnable, PooledThread &thread)
	{
		std::ostringstream msg;
		msg << "runnable " << runnable.name() << " can't be updated, needs to be set up first";
		throw StartException(msg.str());
	}

	RunnableState & RunnableStateCreated::stop(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateCreated::wait(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateCreated::shutdown(Runnable &runnable)
	{
		try
		{
			runnable.shutdown();
		}
		catch (...)
		{
			return m_Manager.changeState("halted");
		}

		return m_Manager.changeState("shut down");
	}

	RunnableState & RunnableStateCreated::handleException(Runnable &runnable)
	{
		runnable.handleException();
		return m_Manager.changeState("halted");
	}
	
	//////////////////////////////////////////////////////////////////////////////////////

	RunnableStateSetUp::RunnableStateSetUp(RunnableManager const& manager) :
		RunnableState(manager, "set up", "this runnable is set up and ready to run")
	{
	}

	RunnableState & RunnableStateSetUp::setup(Runnable &runnable)
	{
		//just call setup again
		runnable.setup();
		return *this;
	}

	RunnableState & RunnableStateSetUp::start(Runnable &runnable, PooledThread &thread)
	{
		thread.start(Poco::Thread::PRIO_NORMAL, runnable);
		return m_Manager.changeState("running");
	}

	RunnableState & RunnableStateSetUp::update(Runnable &runnable, PooledThread &thread)
	{
		thread.update(Poco::Thread::PRIO_NORMAL, runnable);
		return m_Manager.changeState("updating");
	}

	RunnableState & RunnableStateSetUp::stop(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateSetUp::wait(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateSetUp::shutdown(Runnable &runnable)
	{
		runnable.shutdown();
		return m_Manager.changeState("shut down");
	}

	RunnableState & RunnableStateSetUp::handleException(Runnable &runnable)
	{
		runnable.handleException();
		return m_Manager.changeState("halted");
	}

	//////////////////////////////////////////////////////////////////////////////////////

	RunnableStateRunning::RunnableStateRunning(RunnableManager const& manager) :
		RunnableState(manager, "running", "this runnable is running currently")
	{
	}

	RunnableState & RunnableStateRunning::setup(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateRunning::start(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateRunning::update(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateRunning::stop(Runnable &runnable, PooledThread &thread)
	{
		thread.stopTargetRunning();
		return m_Manager.changeState("set up");
	}

	RunnableState & RunnableStateRunning::wait(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateRunning::shutdown(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateRunning::handleException(Runnable &runnable)
	{
		runnable.handleException();
		return m_Manager.changeState("halted");
	}

	//////////////////////////////////////////////////////////////////////////////////////

	RunnableStateUpdating::RunnableStateUpdating(RunnableManager const& manager) :
		RunnableState(manager, "updating", "this runnable is being updated currently")
	{
	}

	RunnableState & RunnableStateUpdating::setup(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateUpdating::start(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateUpdating::update(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateUpdating::stop(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateUpdating::wait(Runnable &runnable, PooledThread &thread)
	{
		//nothing to do...
		return m_Manager.changeState("set up");
	}

	RunnableState & RunnableStateUpdating::shutdown(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateUpdating::handleException(Runnable &runnable)
	{
		runnable.handleException();
		return m_Manager.changeState("halted");
	}

	//////////////////////////////////////////////////////////////////////////////////////

	RunnableStateShutDown::RunnableStateShutDown(RunnableManager const& manager) :
		RunnableState(manager, "shut down", "this runnable was shut down and will be destroyed soon")
	{
	}

	RunnableState & RunnableStateShutDown::setup(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::start(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::update(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::stop(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateShutDown::wait(Runnable &runnable, PooledThread &thread)
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

	//////////////////////////////////////////////////////////////////////////////////////

	RunnableStateHalted::RunnableStateHalted(RunnableManager const& manager) :
		RunnableState(manager, "halted", "an exception occured in the runnable; is is currently halted")
	{
	}

	RunnableState & RunnableStateHalted::setup(Runnable &runnable)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::start(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::update(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::stop(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::wait(Runnable &runnable, PooledThread &thread)
	{
		return *this;
	}

	RunnableState & RunnableStateHalted::shutdown(Runnable &runnable)
	{
		runnable.shutdown();
		return m_Manager.changeState("shut down");
	}

	RunnableState & RunnableStateHalted::handleException(Runnable &runnable)
	{
		return *this;
	}

}