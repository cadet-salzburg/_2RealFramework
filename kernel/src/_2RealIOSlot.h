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

#include "_2RealParameter.h"

#include "Poco/SharedPtr.h"
#include "Poco/Any.h"

namespace _2Real
{

	/**
	*	represents an input or output slot of a service
	*/

	class AbstractRef;
	class ServiceContainer;

	class IOSlot : public Parameter
	{

	public:

		IOSlot(Id *const id, ServiceContainer *const _service, std::string const& type);
		IOSlot(IOSlot const& src);
		IOSlot& operator=(IOSlot const& src);
		virtual ~IOSlot();

		typedef Poco::SharedPtr< Poco::Any >			SharedAny;
		typedef std::pair< unsigned int, SharedAny >	NamedAny;

		/**
		*	true if io slot has been linked
		*/
		const bool										isLinked() const;

		/**
		*	sets abstract ref
		*/
		void											setReference(AbstractRef *const ref);

		/**
		*	returns owning service
		*/
		ServiceContainer *const							service();

		/**
		*	returns owning service - const version
		*/
		ServiceContainer *const							service() const;

		/**
		*	resets linkage
		*/
		virtual void									reset() = 0;

	protected:

		/**
		*	what a dumb name
		*/
		AbstractRef										*m_Ref;

		/**
		*	id of linked slot
		*/
		IOSlot											*m_Linked;

		/**
		*	owning service
		*/
		ServiceContainer								*m_Service;

	};

}