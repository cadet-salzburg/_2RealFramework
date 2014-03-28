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

#include "helpers/_2RealStdIncludes.h"
#include "engine/_2RealAbstractInlet.h"
#include "engine/_2RealIoSlot.h"

namespace _2Real
{
	class Inlet;
	class SharedServiceInletMetainfo;

	class MultiInlet : public AbstractInlet, public std::enable_shared_from_this< MultiInlet >
	{

		typedef std::deque< std::shared_ptr< Inlet > > Inlets;

	public:

		explicit MultiInlet( std::shared_ptr< const SharedServiceInletMetainfo > );

		uint32_t getSize() const;
		bool isEmpty() const;
		std::shared_ptr< Inlet > operator[]( const uint32_t );
		std::shared_ptr< Inlet > add_front();
		std::shared_ptr< Inlet > add_back();

		using AbstractInlet::getName;
		using AbstractInlet::isMultiInlet;
		//using AbstractInlet::getMetainfo;

		void update();

	private:

		mutable std::mutex									mMutex;
		Inlets												mInlets;
		std::shared_ptr< SharedServiceInletMetainfo >		mInfo;

	};

}