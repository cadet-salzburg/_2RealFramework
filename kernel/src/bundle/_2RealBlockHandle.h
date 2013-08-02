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

namespace _2Real
{
	class FunctionBlockIOManager;

	namespace bundle
	{
		class InletHandle;
		class OutletHandle;
		class ParameterHandle;

		class BlockHandle
		{

		public:

			typedef std::vector< InletHandle >						InletHandles;
			typedef std::vector< InletHandle >::iterator			InletHandleIterator;
			typedef std::vector< InletHandle >::const_iterator		InletHandleConstIterator;

			typedef std::vector< OutletHandle >						OutletHandles;
			typedef std::vector< OutletHandle >::iterator			OutletHandleIterator;
			typedef std::vector< OutletHandle >::const_iterator		OutletHandleConstIterator;

			typedef std::vector< ParameterHandle >					ParameterHandles;
			typedef std::vector< ParameterHandle >::iterator		ParameterHandleIterator;
			typedef std::vector< ParameterHandle >::const_iterator	ParameterHandleConstIterator;

			BlockHandle();
			explicit BlockHandle( std::shared_ptr< FunctionBlockIOManager > );

			bool isValid() const;

			InletHandle				getInletHandle( std::string const& );
			OutletHandle			getOutletHandle( std::string const&);
			ParameterHandle			getParameterHandle( std::string const& );
			void					getAllInletHandles( InletHandles & );
			void					getAllOutletHandles( OutletHandles & );
			void					getAllParameterHandles( ParameterHandles & );

		private:

			std::weak_ptr< FunctionBlockIOManager >		mImpl;

		};
	}
}