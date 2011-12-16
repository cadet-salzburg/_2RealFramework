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

#include "_2RealRunnableGraph.h"

namespace _2Real
{

	class SystemGraph;

	class Sequence : public RunnableGraph
	{

	public:

		Sequence(Identifier const& id, SystemGraph &system);
		~Sequence();;

		void start(bool runOnce);
		void stop();
		void setup();
		void run();
		void update();
		void shutdown();

		void insert(RunnableManager &child, unsigned int index);
		void remove(Identifier const& id);

	};

}