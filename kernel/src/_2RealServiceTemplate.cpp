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

#include "_2RealServiceTemplate.h"
#include "_2RealPlugin.h"
#include "_2RealServiceMetadata.h"
#include "_2RealException.h"

namespace _2Real
{

	ServiceTemplate::ServiceTemplate(std::string const& _name, unsigned int const& _pluginID, ServiceCreator _creator, ServiceMetadata const& _metadata, Id *const _id) :
		Entity(_id),
		m_Name(_name),
		m_Plugin(_pluginID),
		m_Creator(_creator),
		m_Metadata(_metadata)
	{
	}

	ServiceTemplate::ServiceTemplate(ServiceTemplate const& _src) :
		Entity(_src),
		m_Metadata(_src.m_Metadata),
		m_Plugin(0),
		m_Name(""),
		m_Creator(NULL)
	{
		throw Exception("internal error: attempted to copy an entity");
	}

	ServiceTemplate& ServiceTemplate::operator=(ServiceTemplate const& _src)
	{
		throw Exception("internal error: attempted to copy an entity");
	}

	ServiceTemplate::~ServiceTemplate()
	{
	}

	unsigned int const& ServiceTemplate::plugin() const
	{
		return m_Plugin;
	}

	ServiceMetadata const& ServiceTemplate::metadata() const
	{
		return m_Metadata;
	}

	std::string const& ServiceTemplate::name() const
	{
		return m_Name;
	}

	IService *const ServiceTemplate::create()
	{
		return m_Creator();
	}

}