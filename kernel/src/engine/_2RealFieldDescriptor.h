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

#include "engine/_2RealAnyHolder_T.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class AbstractFieldDescriptor
	{
	public:
		virtual ~AbstractFieldDescriptor() {}
		virtual std::shared_ptr< AbstractAnyHolder > makeAny() const = 0;
		virtual void enumerateFields() = 0;
	};

	template< typename TType >
	class FieldDescriptor_T : public AbstractFieldDescriptor
	{
	public:
		FieldDescriptor_T( TType const& init ) : mInit( new TType( init ) ) {}
		std::shared_ptr< AbstractAnyHolder > makeAny() const { return std::shared_ptr< AbstractAnyHolder >( new AnyHolder_T< TType >( *mInit.get() ) ); }
		void enumerateFields() {}
	private:
		FieldDescriptor_T( FieldDescriptor_T const& other );
		FieldDescriptor_T& operator=( FieldDescriptor_T const& other );
		std::shared_ptr< TType > mInit;
		// TODO: shared ptr?
	};
}
