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

#include "_2RealIEntity.h"

#include "Poco/SharedPtr.h"
#include "Poco/Any.h"

namespace _2Real
{

	/**
	*
	*/

	class ServiceImpl;
	class AbstractRef;

	class ServiceParam : public IEntity
	{

	public:

		/**
		*	
		*/
		typedef Poco::SharedPtr< Poco::Any >						SharedAny;

		/**
		*	
		*/
		typedef std::pair< const unsigned int, const SharedAny >	NamedAny;

		/**
		*	
		*/
		ServiceParam(IdentifierImpl *const _id, ServiceImpl const *const _service);

		/**
		*	
		*/
		ServiceParam(ServiceParam const& _src) throw(...);

		/**
		*	
		*/
		ServiceParam& operator=(ServiceParam const& _src) throw(...);

		/**
		*	
		*/
		~ServiceParam();

		/**
		*	
		*/
		ServiceImpl const *const service() const;

		/**
		*	
		*/
		unsigned int const& senderID() const;

		/**
		*	
		*/
		void setValue(AbstractRef *const _val);

		/**
		*	
		*/
		void listenTo(unsigned int const& _id);

		/**
		*	
		*/
		ServiceParam::NamedAny getAny();

		/**
		*
		*/
		void extractFrom(ServiceParam::SharedAny const& _any) throw(...);

	private:

		/**
		*	
		*/
		const ServiceImpl			*m_Service;

		/**
		*	what a dumb name
		*/
		AbstractRef					*m_Value;

		/**
		*	
		*/
		unsigned int				m_SenderID;

	};

}