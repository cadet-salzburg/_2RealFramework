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

#include "_2RealTypedefs.h"

#include "_2RealAbstractServiceVariable.h"

#include <string>

namespace _2Real
{
	template< typename T >
	class ServiceVariable : public AbstractServiceVariable
	{

	public:

		ServiceVariable(std::string const& _name, T &_var);
		
		const bool getFrom(const Data &_data);
		const bool insertInto(Data &_data) const;

	private:

		T		&m_Variable;

	};

	template< typename T >
	ServiceVariable< T >::ServiceVariable(std::string const& _name, T &_var) :
		AbstractServiceVariable(_name), m_Variable(_var)
	{
	}

	template< typename T >
	const bool ServiceVariable< T >::getFrom(const Data &_data)
	{
		Poco::Any any;
		if (AbstractServiceVariable::getFrom(_data, any))
		{
			try
			{
				m_Variable = Poco::AnyCast< T >(any);
			}
			catch (Poco::BadCastException e)
			{
				std::cout << "TODO: error handling; bad cast on ServiceVariable::getFrom" << std::endl;
				return false;
			}
		}

		return true;
		//return _data.get< T >(AbstractServiceVariable::frameworkName(), m_Variable);
	}

	template< typename T >
	const bool ServiceVariable< T >::insertInto(Data &_data) const
	{
		Poco::Any any(m_Variable);
		return AbstractServiceVariable::insertInto(_data, any);
		//_data.insert< T >(AbstractServiceVariable::frameworkName(), m_Variable);
	}
}