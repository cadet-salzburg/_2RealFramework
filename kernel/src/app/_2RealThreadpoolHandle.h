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

namespace _2Real
{
	class ThreadpoolImpl_I;

	/*
	*	new: added handle to threadpool to fw ( = concept of 'system' from earliest design )
	*	created & managed by engine
	*	a threadpool must exist to be able to create block instances
	*	other than this, there is no functionality here ( yet )
	*	all threadpools are removed once the engine is destroyed
	*/

	namespace app
	{
		class ThreadpoolHandle
		{

		public:

			/*
			*	created by engine via Engine::createThreadpool
			*/
			explicit ThreadpoolHandle( std::shared_ptr< ThreadpoolImpl_I > );

			/*
			*	@return : true if underlying object is valid ( all threadpools are destroyed once the engine is destroyed )
			*/
			bool isValid() const;

			// TODO
			// destroy of sorts
			//	-> what happens to the blocks then
			// resize
			// etc.

		private:

			friend class BundleHandle;

			std::weak_ptr< ThreadpoolImpl_I >		mImpl;

		};
	}
}