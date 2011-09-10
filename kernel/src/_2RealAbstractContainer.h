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

#include "_2RealEngineTypedefs.h"
#include "_2RealIContainer.h"
#include "_2RealIEntity.h"

#include <list>

#include "Poco/BasicEvent.h"

namespace _2Real
{

	/**
	*	implements some container functionality, and adds some
	*/

	class DataQueue;
	class ServiceSlot;
	class Exception;

	class AbstractContainer : public Entity, public IContainer
	{

	public:

		/**
		*	
		*/
		AbstractContainer(IdentifierImpl *const _id);

		/**
		*	
		*/
		AbstractContainer(AbstractContainer const& _src) throw(...);

		/**
		*	
		*/
		AbstractContainer& operator=(AbstractContainer const& _src) throw(...);

		/**
		*	
		*/
		virtual ~AbstractContainer();

		/**
		*	returns pointer to production graph root
		*/
		AbstractContainer *const root();

		/**
		*	prepares for run()
		*/
		void start(bool const& _runOnce) throw(...);

		/**
		*	stops run()
		*/
		void stop();

		/**
		*	
		*/
		bool const& canReconfigure() const;

		/**
		*	
		*/
		void setup(ServiceContext *const _contextPtr) throw(...);

		/**
		*	
		*/
		void addListener(IDataQueue *const _queue) throw(...);
		
		/**
		*	
		*/
		void removeListener(IDataQueue *const _queue) throw(...);
		
		/**
		*	
		*/
		void receiveData(NamedData &_data);

		/**
		*	
		*/
		void sendData(bool const& _blocking);

		/**
		*	returns father
		*/
		AbstractContainer *const father();

		/**
		*	sets father, throws exception if father is null
		*/
		void setFather(AbstractContainer *const _father) throw(...);

		/**
		*	
		*/
		virtual IDs setupParamIDs() const throw(...) = 0;

		/**
		*	
		*/
		virtual IDs inputSlotIDs() const throw(...) = 0;

		/**
		*	
		*/
		virtual IDs outputSlotIDs() const throw(...) = 0;

		/**
		*	
		*/
		virtual std::list< ServiceSlot * > inputSlots() throw(...) = 0;

		/**
		*	
		*/
		virtual std::list< ServiceSlot * > outputSlots() throw(...) = 0;

		/**
		*	
		*/
		void listenTo(AbstractContainer *const _sender) throw(...);

		/**
		*	
		*/
		void stopListeningTo(AbstractContainer *const _sender) throw(...);

		/**
		*	
		*/
		void resetIO();

	protected:

		Poco::Mutex									m_Mutex;

		/**
		*	true if container is running once (child of sync)
		*/
		bool										m_bRunOnce;

		/**
		*	true is container is running threaded (child of nirvana)
		*/
		bool										m_bRun;

		/**
		*	will be true after the first call tho check configuration
		*/
		bool										m_bIsConfigured;

		/**
		*	true if setup params can be changed
		*/
		bool										m_bCanReconfigure;

		/**
		*	father - null for top level containers
		*/
		AbstractContainer							*m_Father;

		/**
		*	list of received DataImpl items
		*/
		std::list< NamedData >						m_DataList;

		/**
		*	used to send new data to listeners
		*/
		Poco::BasicEvent< NamedData >				m_NewData;

		/**
		*	used to notify listeners of exceptions
		*/
		Poco::BasicEvent< Exception >				m_Exception;

		/**
		*	
		*/
		typedef std::list< AbstractContainer * >	ContainerList;

		/**
		*	all containers that this one listens to
		*/
		ContainerList								m_Senders;

		/**
		*	all containers that listen to this one
		*/
		ContainerList								m_Listeners;

		/**
		*	allows listening to the container from the outside
		*/
		DataQueue									*m_OutputQueue;
	};

}