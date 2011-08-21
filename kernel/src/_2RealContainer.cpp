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

#include "_2RealContainer.h"
#include "_2RealException.h"

namespace _2Real
{
	
	Container::Container(AbstractContainer *const _father, IdentifierImpl *const _id) : AbstractContainer(_father, _id)
	{
	}

	Container::Container(Container const& _src) : AbstractContainer(_src)
	{
	}

	Container& Container::operator=(Container const& _src)
	{
		throw Exception::noCopy();
	}

	Container::~Container()
	{
		try
		{
			shutdown();
		}
		catch (...)
		{
			std::cout << "service shutdown failed" << std::endl;
		}
		
		//TODO: children are deleted here
	}

	void Container::configure(ConfigurationData *const _config)
	{
		if (m_bIsConfigured && !m_bCanReconfigure)
		{
			//TODO: set error state
			throw Exception::failure();
		}
		else if (_config == NULL)
		{
			//TODO: set error state
			throw Exception::failure();
		}

		try
		{
			//TODO: setup children here
		}
		catch (...)
		{
			throw Exception::failure();
		}

		//save configuration
		m_Configuration = _config;
	}

	void Container::run()
	{
		while (m_bIsConfigured && (m_bRun || m_bRunOnce))
		{
			try
			{
			}
			catch (...)
			{

				throw Exception::failure();
			}

			m_bRunOnce = false;
			sendData(false);
		}
	}

	void Container::update()
	{
		if (!m_bIsConfigured)
		{
			throw Exception::failure();
		}

		try
		{

		}
		catch (...)
		{
			throw Exception::failure();
		}

		sendData(true);
	}

	void Container::shutdown()
	{
		try
		{
		}
		catch(...)
		{
			//TODO: set error state
			throw Exception::failure();
		}
	}
}