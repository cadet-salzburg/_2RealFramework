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

#include "_2RealFactoryReference.h"
#include "_2RealPlugin.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"

namespace _2Real
{

	FactoryReference::FactoryReference(std::string const& _name, Plugin const *const _plugin, ServiceCreator _creator, Metadata const *const _metadata, IdentifierImpl *const _id) :
		IEntity(_id), m_Name(_name), m_Plugin(_plugin), m_Creator(_creator), m_Metadata(_metadata)
	{
	}

	FactoryReference::FactoryReference(FactoryReference const& _src) : 
	IEntity(_src), m_Plugin(NULL), m_Metadata(NULL), m_Creator(NULL)
	{
		throw Exception::noCopy();
	}

	FactoryReference& FactoryReference::operator=(FactoryReference const& _src)
	{
		throw Exception::noCopy();
	}

	FactoryReference::~FactoryReference()
	{
	}

	const bool FactoryReference::canReconfigure() const
	{
		//TODO
		return false;
	}

	const bool FactoryReference::isSingleton() const
	{
		//TODO
		return false;
	}

	const bool FactoryReference::canCreate() const
	{
		return (m_Plugin != NULL && m_Plugin->state() == Plugin::ACTIVE);
	}

	Plugin const *const FactoryReference::plugin() const
	{
		return m_Plugin;
	}

	Metadata const *const FactoryReference::metadata() const
	{
		return m_Metadata;
	}

	IService *const FactoryReference::create()
	{
		return m_Creator();
	}

	std::string const& FactoryReference::name() const
	{
		return m_Name;
	}

}