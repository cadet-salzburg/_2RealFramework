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

#include <string>

namespace _2Real
{

	class Runnable;
	class RunnableManager;
	class PooledThread;

	class RunnableState
	{

	public:

		RunnableState(RunnableManager const& manager, std::string const& description);
		std::string const& getDescription();
		virtual ~RunnableState();
		virtual RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce) = 0;
		virtual RunnableState & update(Runnable &runnable) = 0;
		virtual RunnableState & stop(Runnable &runnable) = 0;
		virtual RunnableState & setup(Runnable &runnable) = 0;
		virtual RunnableState & shutdown(Runnable &runnable) = 0;
		virtual RunnableState & handleException(Runnable &runnable) = 0;

	protected:

		std::string			const m_Description;
		RunnableManager		const& m_Manager;

	};

	inline std::string const& RunnableState::getDescription()
	{
		return m_Description;
	}

	class RunnableStateCreated : public RunnableState
	{

	public:

		RunnableStateCreated(RunnableManager const& manager);
		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce);
		RunnableState & update(Runnable &runnable);
		RunnableState & stop(Runnable &runnable);
		RunnableState & setup(Runnable &runnable);
		RunnableState & shutdown(Runnable &runnable);
		RunnableState & handleException(Runnable &runnable);

	};

	class RunnableStateSetUp : public RunnableState
	{

	public:

		RunnableStateSetUp(RunnableManager const& manager);
		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce);
		RunnableState & update(Runnable &runnable);
		RunnableState & stop(Runnable &runnable);
		RunnableState & setup(Runnable &runnable);
		RunnableState & shutdown(Runnable &runnable);
		RunnableState & handleException(Runnable &runnable);

	};

	class RunnableStateStarted : public RunnableState
	{

	public:

		RunnableStateStarted(RunnableManager const& manager);
		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce);
		RunnableState & update(Runnable &runnable);
		RunnableState & stop(Runnable &runnable);
		RunnableState & setup(Runnable &runnable);
		RunnableState & shutdown(Runnable &runnable);
		RunnableState & handleException(Runnable &runnable);

	};

	class RunnableStateHalted : public RunnableState
	{

	public:

		RunnableStateHalted(RunnableManager const& manager);
		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce);
		RunnableState & update(Runnable &runnable);
		RunnableState & stop(Runnable &runnable);
		RunnableState & setup(Runnable &runnable);
		RunnableState & shutdown(Runnable &runnable);
		RunnableState & handleException(Runnable &runnable);

	};

	class RunnableStateShutDown: public RunnableState
	{

	public:

		RunnableStateShutDown(RunnableManager const& manager);
		RunnableState & start(Runnable &runnable, PooledThread &thread, bool runOnce);
		RunnableState & update(Runnable &runnable);
		RunnableState & stop(Runnable &runnable);
		RunnableState & setup(Runnable &runnable);
		RunnableState & shutdown(Runnable &runnable);
		RunnableState & handleException(Runnable &runnable);

	};

}