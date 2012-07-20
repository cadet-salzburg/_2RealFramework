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

#include "engine/_2RealInlet.h"
#include "bundle/_2RealInletHandle.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealEngineImpl.h"

using std::string;

namespace _2Real
{
	Inlet::Inlet( AbstractUberBlock &owningBlock, string const& name, string const& longTypename, string const& type ) :
		Parameter( longTypename, type ),
		NonCopyable< Inlet >(),
		Identifiable< Inlet >( owningBlock.getIds(), name ),
		Handleable< Inlet, bundle::InletHandle >( *this ),
		m_Engine( EngineImpl::instance() ),
		m_OwningUberBlock( owningBlock )
	{
	}

	void Inlet::setDataAndSynchronize( TimestampedData const& data )
	{
		Parameter::setData( data );
		Parameter::synchronize();
	}

	AbstractUberBlock & Inlet::getOwningUberBlock()
	{
		return m_OwningUberBlock;
	}
}