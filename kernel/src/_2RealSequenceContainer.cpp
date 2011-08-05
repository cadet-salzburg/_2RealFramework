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

#include "_2RealSequenceContainer.h"
#include "_2RealServiceContainer.h"

namespace _2Real
{
	void SequenceContainer::update()
	{
		if (m_bIsConfigured && !m_bRunThreaded)
		{
			for (ContainerList::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
			{
				//just update children sequentially
				(*it)->update();
			}
		}
	}

/*
	void SequenceContainer::startChildren(bool _startAll)
	{
		if (!m_bIsConfigured)
		{
			return;
		}

		if (_startAll)
		{
			//all containers run threaded
			for (ContainerSequence::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
			{
				(*it)->start(false);
				m_ThreadPool.start(*it->get());
			}
		}
		else
		{
			//service containers -> started in threaded modus
			//group containers -> all service children started in threaded modus
			for (ContainerSequence::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
			{
				if (dynamic_cast< ServiceContainer* >(it->get()))
				{
					(*it)->start(false);
					m_ThreadPool.start(*it->get());
				}
				else
				{
					//(*it)->startChildren(false);
				}
			}
		}
	}

	void SequenceContainer::stopChildren(bool _stopAll)
	{
	}
*/

}