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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealData.h"

namespace _2Real
{
	class ParameterImpl;

	namespace app
	{
		class BlockHandle;

		class ParameterHandle
		{

		public:

			/*
			*	created by block handle via BlockHandle::getParameter
			*/
			explicit ParameterHandle( std::shared_ptr< ParameterImpl > );

			/*
			*	@return: true id underlying object is valid ( all io slots are destroied once the parent block is removed ) 
			*/
			bool				isValid() const;

			/*
			*	@return: handle to parent block
			*/
			BlockHandle			getBlock();

			/*
			*	sets the parameter value
			*/
			void				setValue( DataItem value );

		private:

			std::weak_ptr< ParameterImpl >		mImpl;

		};
	}
}
