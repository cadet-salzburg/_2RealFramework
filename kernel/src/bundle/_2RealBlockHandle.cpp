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

#include "bundle/_2RealBlockHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "engine/_2RealFunctionBlockIoManager.h"

using std::string;

namespace _2Real
{
	namespace bundle
	{
		BlockHandle::BlockHandle( FunctionBlockIOManager &block ) :
			m_Impl( block )
		{
		}

		InletHandle & BlockHandle::getInletHandle( string const& name ) const
		{
			return m_Impl.getBundleInletHandle( name );
		}

		OutletHandle & BlockHandle::getOutletHandle( string const& name ) const
		{
			return m_Impl.getBundleOutletHandle( name );
		}

		BlockHandle::InletHandles const& BlockHandle::getAllInletHandles() const
		{
			return m_Impl.getBundleInletHandles();
		}

		BlockHandle::OutletHandles const& BlockHandle::getAllOutletHandles() const
		{
			return m_Impl.getBundleOutletHandles();
		}
	}
}