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

#include "_2RealMetadata.h"
#include "_2RealPluginMetadata.h"
#include "_2RealTypeHolder.h"

#include <sstream>

#include "Poco/SharedPtr.h"

namespace _2Real
{

	Metadata::Metadata(PluginMetadata &metadata) :
		m_Impl(metadata)
	{
	}

	void Metadata::test(float f)
	{
		//float *x = new float(f);
		//m_Impl.m_Test.assign(x);
		AbstractDataHolder *holder = new DataHolder< float >(f);
		m_Impl.m_Test.assign(holder);
	}

	void Metadata::setDescription(std::string const& description)
	{
		m_Impl.setDescription(description);
	}

	void Metadata::setVersion(unsigned int major, unsigned int minor, unsigned int revision)
	{
		m_Impl.setVersion(Version(major, minor, revision));
	}

	void Metadata::setAuthor(std::string const& author)
	{
		m_Impl.setAuthor(author);
	}

	void Metadata::setContact(std::string const& contact)
	{
		m_Impl.setContact(contact);
	}

	void Metadata::addService(std::string const& serviceName)
	{
		m_Impl.addServiceMetadata(serviceName);
	}

	void Metadata::setDescription(std::string const& serviceName, std::string const& description)
	{
		m_Impl.setDescription(serviceName, description);
	}

	void Metadata::addSetupParameterByType(std::string const& setupName, std::string const& type)
	{
		m_Impl.addSetupParameterByType(setupName, type);
	}

	void Metadata::addSetupParameterByType(std::string const& serviceName, std::string const& setupName, std::string const& type)
	{
		m_Impl.addSetupParameterByType(serviceName, setupName, type);
	}

	void Metadata::addInputSlotByType(std::string const& serviceName, std::string const& inputName, std::string const& type)
	{
		m_Impl.addInputSlotByType(serviceName, inputName, type);
	}

	void Metadata::addOutputSlotByType(std::string const& serviceName, std::string const& outputName, std::string const& type)
	{
		m_Impl.addOutputSlotByType(serviceName, outputName, type);
	}

	void Metadata::addSetupParameterByData(std::string const& setupName, EngineData const& defaultValue)
	{
		m_Impl.addSetupParameterByData(setupName, defaultValue);
	}

	void Metadata::addSetupParameterByData(std::string const& serviceName, std::string const& setupName, EngineData const& defaultValue)
	{
		m_Impl.addSetupParameterByData(serviceName, setupName, defaultValue);
	}

	void Metadata::addInputSlotByData(std::string const& serviceName, std::string const& inputName, EngineData const& defaultValue)
	{
		m_Impl.addInputSlotByData(serviceName, inputName, defaultValue);
	}

}