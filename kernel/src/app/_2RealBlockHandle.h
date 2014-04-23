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
#include "common/_2RealBlockResult.h"
#include "common/_2RealException.h"
#include "common/_2RealSignals.h"

namespace _2Real
{
	class BlockImpl;

	namespace app
	{
		class BundleHandle;
		class TimerHandle;
		class UpdatePolicyHandle;
		class BlockIo;
		class InletHandle;
		class MultiInletHandle;
		class OutletHandle;
		class ParameterHandle;

		class BlockHandle
		{

		public:

			explicit BlockHandle( std::shared_ptr< BlockImpl > );

			bool							isValid() const;

			BundleHandle					getBundle();

			std::future< BlockResult >		setup();
			std::future< BlockResult >		singlestep();
			std::future< BlockResult >		shutdown();
			std::future< BlockResult >		startUpdating( TimerHandle );
			std::future< BlockResult >		startUpdating();
			std::future< BlockResult >		stopUpdating();
			void							destroy( const uint64_t );

			UpdatePolicyHandle				getUpdatePolicy();

			BlockIo							getBlockIo();
			InletHandle						getInlet( std::string const& );
			MultiInletHandle				getMultiInlet( std::string const& );
			OutletHandle					getOutlet( std::string const& );
			ParameterHandle					getParameter( std::string const& );

			Connection						registerToException( boost::function< void( Exception ) > ); 

		private:

			friend class BundleHandle;

			std::weak_ptr< BlockImpl >		mImpl;

		};
	}
}
