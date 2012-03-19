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
#include <iostream>

namespace _2Real
{

	class Runnable;
	class RunnableManager;
	class PooledThread;

	class AbstractRunnableState
	{

	public:

		AbstractRunnableState(const unsigned int stateId, std::string const& description);
		unsigned int const& getStateId();
		std::string const& getDescription();

		virtual ~AbstractRunnableState();

		virtual void setUp(Runnable &runnable) const = 0;
		virtual const bool tryGetReady() const = 0;
		virtual const bool tryBeginUpdate() const = 0;
		virtual const bool tryFinishUpdate() const = 0;
		virtual const bool shutDown(Runnable &runnable) const = 0;

	protected:

		unsigned int		const m_StateId;
		std::string			const m_Description;

	};

}