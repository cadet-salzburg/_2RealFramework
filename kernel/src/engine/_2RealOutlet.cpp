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

#include "engine/_2RealOutlet.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealEngineImpl.h"
#include "datatypes/_2RealCustomData.h"

using std::string;
using std::ostringstream;

namespace _2Real
{
	Outlet::Outlet( AbstractUberBlock &owningBlock, string const& name, std::shared_ptr< const CustomType > initialValue ) :
		Parameter( initialValue ),
		NonCopyable< Outlet >(),
		Identifiable< Outlet >( owningBlock.getIds(), name ),
		Handleable< Outlet, bundle::OutletHandle >( *this ),
		m_Engine( EngineImpl::instance() ),
		m_OwningUberBlock( owningBlock ),
		m_DiscardCurrent( false ),
		mInit( initialValue )
	{
		// the 'initialValue' is just a template, I now need to clone that twice
		//Parameter::m_Data->cloneFrom( *( initialValue.get() ) );
		//Parameter::m_DataBuffer->cloneFrom( *( initialValue.get() ) );
	}

	bool Outlet::synchronize()
	{
		if ( !m_DiscardCurrent )
		{
			// store time
			m_Timestamp = m_Engine.getElapsedTime();
			// data = buffer, protected by mutex ( could be read by client right now )
			Parameter::synchronize();
			// clone readable data back into writeable data
			// this way, outlet always holds the last written value
			//Parameter::m_DataBuffer.reset( new CustomType( *Parameter::m_Data.get() ) );		// clone
			Parameter::m_DataBuffer.reset( new CustomType( *mInit.get() ) );		// clone
			return false;
		}
		else
		{
			m_DiscardCurrent = false;
			return true;
		}
	}

	//CustomType & Outlet::getWriteableData()
	//{
	//	return Parameter::getWriteableData();
	//}

	void Outlet::discardCurrentUpdate()
	{
		m_DiscardCurrent = true;
	}

	AbstractUberBlock & Outlet::getOwningUberBlock()
	{
		return m_OwningUberBlock;
	}
}