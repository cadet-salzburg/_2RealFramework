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

#include "Poco/SharedPtr.h"

namespace _2Real
{

	/**
	*
	*/

	template < typename Key >
	class Data;

	class VariableName;

	class ContainerName;

	typedef Data< VariableName >						VariableData;

	typedef Poco::SharedPtr< VariableData >				SharedDataPtr;

	typedef std::pair< ContainerName, SharedDataPtr >	NamedData;
	
	class IDataQueue
	{

	public:

		/**
		*	adds other queue to listeners
		*	does nothing if queue already listens
		*/
		virtual void addListener(IDataQueue *const _queue) = 0;
		
		/**
		*	removes other queue from listeners
		*	does nothng if other queue is not actually a listener
		*/
		virtual void removeListener(IDataQueue *const _queue) = 0;
		
		/**
		*	function to receive data
		*/
		virtual void receiveData(NamedData &_data) = 0;

		/**
		*	function to send out data
		*	@param _blocking - whether or not the function returns immediately after notifying its listeners
		*/
		virtual void sendData(bool const& _blocking) = 0;

	};

}