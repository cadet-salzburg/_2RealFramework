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

#include "_2RealServiceParam.h"

#include "Poco/SharedPtr.h"
#include "Poco/Any.h"

namespace _2Real
{

	/**
	*	represents an input or output slot of a service
	*/

	class ServiceContainer;
	class AbstractRef;

	class ServiceSlot : public ServiceParam
	{

	public:

		/**
		*	
		*/
		typedef Poco::SharedPtr< Poco::Any >						SharedAny;

		/**
		*	
		*/
		typedef std::pair< unsigned int, SharedAny >				NamedAny;

		/**
		*	
		*/
		ServiceSlot(Id *const _id, ServiceContainer *const _service, std::string const& _type);

		/**
		*	
		*/
		ServiceSlot(ServiceSlot const& _src) throw(...);

		/**
		*	
		*/
		ServiceSlot& operator=(ServiceSlot const& _src) throw(...);

		/**
		*	
		*/
		~ServiceSlot();

		/**
		*	return sender id
		*/
		ServiceSlot *const linked();

		/**
		*	link with other slot
		*/
		void linkWith(ServiceSlot *const _link);

		/**
		*	true if sender id has been set
		*/
		bool const& isLinked() const;

		/**
		*	set abstract ref
		*/
		void setValue(AbstractRef *const _val);

		/**
		*	return value as any (output slot)
		*/
		ServiceSlot::NamedAny getAny();

		/**
		*	get value from any (input slot)
		*/
		void extractFrom(ServiceSlot::SharedAny const& _any) throw(...);

		/**
		*	reset sender
		*/
		void reset();

	private:

		/**
		*	what a dumb name
		*/
		AbstractRef					*m_Value;

		/**
		*	id of corresponding output slot
		*/
		ServiceSlot					*m_Linked;

		/**
		*	true if sender id has been set
		*/
		bool						m_bIsLinked;

	};

}