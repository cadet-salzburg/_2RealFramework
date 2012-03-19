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

	class UpdateTriggersImpl;

	class UpdateTriggers
	{

	public:

		static UpdateTriggers& ServiceDefault();
		static UpdateTriggers& GraphDefault();
		static UpdateTriggers& EmptyTriggers();

		/**
		*	creates an empty set of triggers
		*	creating a service with this will cause the service to nver be updated, ever
		*/
		UpdateTriggers();
		~UpdateTriggers();

		/**
		*	empties the triggers
		*/
		void clear();

		/**
		*	adds a time based trigger; there can be only one per runnable so calling this multiple times
		*	will overwrite the old one
		*/
		void triggerByUpdateRate(const float updatesPerSecond);

		/*
		*	'available' and 'new' are mutually exclusive per inlet
		*/
		void triggerWhenAllDataNew();
		void triggerWhenAllDataAvailable();
		//void triggerWhenDataNew(std::string const& inletName);
		//void triggerWhenDataAvailable(std::string const& inletName);

		/**
		*	adds child based triggers. obviously this only works for non-services
		*/
		void triggerWhenAllChildrenReady();
		//void triggerWhenChildReady(std::string const& childName);

	private:

		friend class SystemImpl;

		UpdateTriggers(UpdateTriggers const& src);
		UpdateTriggers& operator=(UpdateTriggers const& src);

		UpdateTriggersImpl			*m_Impl;

	};

}