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

			/*
			*	created by bundle handle via BundleHandle::createBlock
			*/
			explicit BlockHandle( std::shared_ptr< BlockImpl > );
			
			/*
			*	@return: true if underlying object is valid ( BlockHandle::destroy or BundleHandle::unload or engine destruction will remove blocks )
			*/
			bool							isValid() const;

			/*
			*	@return: handle to parent bundle
			*/
			BundleHandle					getBundle();

			/*
			*	@state changes
			*	basically, you create a block, you call setup, and then you may update it,
			*	either by calling singlestep repeatedly or by calling startUpdating
			*	all functions are non-blocking and return a std::future
			*/

			/*
			*	attempts to set up block: this request is carried out if the underlying block is not currently running
			*	and has not been shut down already
			*/
			std::future< BlockResult >		setup();

			/*
			*	attempts to carry out a single update: this request is carried out if the underlying block has been set up,
			*	has not yet been shutdown and is not currently running
			*/
			std::future< BlockResult >		singlestep();

			/*
			*	attempts to shut down block: this request is carried out if the underlying block is not currently running
			*/
			std::future< BlockResult >		shutdown();

			/*
			*	attempts to 'start' the block, meaning it will update based on the timer signal; must have been set up and not shut down yet
			*/
			std::future< BlockResult >		startUpdating( TimerHandle );

			/*
			*	attempts to 'start' the block, meaning it will update based on the update policy; must have been set up and not shut down yet
			*/
			std::future< BlockResult >		startUpdating();

			/*
			*	if the block has been started, it will stop 
			*/
			std::future< BlockResult >		stopUpdating();

			/*
			*	attempts to shut the block down, if necessary, and removes it
			*	blocks up to timeout milliseconds, throws an exception if the shutdown has not happened until then
			*/
			void							destroy( const uint64_t timeout );

			/*
			*	@return: a handle to the update policy
			*/
			UpdatePolicyHandle				getUpdatePolicy();

			/*
			*	@return: a collection of all inlet handles, outlet handles and parameter handles
			*/
			BlockIo							getBlockIo();

			/*
			*	@return: handle to an inlet
			*/
			InletHandle						getInlet( std::string const& );

			/*
			*	@return: handle to a subinlet
			*/
			MultiInletHandle				getMultiInlet( std::string const& );

			/*
			*	@return: handle to an outlet
			*/
			OutletHandle					getOutlet( std::string const& );

			/*
			*	@return: handle to a parameter
			*/
			ParameterHandle					getParameter( std::string const& );

			/*
			*	allows you to register a function taht is called if the block instance throws an exception in setup/update/shutdown
			*	( consider using boost::bind, but std::bind also works fine )
			*	@return: the connection
			*/
			Connection						registerToException( boost::signals2::signal< void( Exception ) >::slot_type ); 

		private:

			friend class BundleHandle;

			std::weak_ptr< BlockImpl >		mImpl;

		};
	}
}
