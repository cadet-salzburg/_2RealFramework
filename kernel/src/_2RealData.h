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

#include "_2RealException.h"
#include "_2RealIdentifier.h"
#include "_2RealSharedAny.h"

#include "Poco\Timestamp.h"

namespace _2Real
{

	class Data
	{

	public:

		virtual ~Data()
		{
		}

		//Identifier const& id() const
		//{
		//	return m_Id;
		//}

		template< typename Datatype >
		Datatype const& getData()
		{
			Poco::SharedPtr< Datatype > ptr = Extract< Datatype >(data());
			return *ptr.get();
		}

		SharedAny const& data() const
		{
			return m_Data;
		}

		Poco::Timestamp const& timestamp() const
		{
			return m_Timestamp;
		}

	private:

		friend class OutputSlot;

		Data() :
			m_Data(SharedAny()),
			m_Timestamp(Poco::Timestamp())
		{
		}

		//Data(Identifier const& id, SharedAny const& data, Poco::Timestamp const& time) :
		//	m_Id(id),
		//	m_Data(data),
		//	m_Timestamp(time)
		//{
		//}

		Data(SharedAny const& data, Poco::Timestamp const& time) :
			m_Data(data),
			m_Timestamp(time)
		{
		}

		//Identifier			const m_Id;
		SharedAny			m_Data;
		Poco::Timestamp		m_Timestamp;

	};

	//class DataHandle : public ParameterHandle
	//{

	//public:

	//	DataHandle();
	//	DataHandle(InputHandle const& src);
	//	DataHandle& operator=(DataHandle const& src);
	//	~DataHandle();

	//	template< typename Datatype >
	//	Datatype const& data()
	//	{
	//		if (!m_Output)
	//		{
	//			throw BadHandleException(m_Name);
	//		}

	//		try
	//		{
	//			Poco::SharedPtr< Datatype > ptr = Extract< Datatype >();
	//			return *ptr.get();
	//		}
	//		catch (Exception &e)
	//		{
	//			throw e;
	//		}
	//	}

	//private:

	//	friend class OutputSlot;

	//	DataHandle(OutputSlot *slot);

	//	SharedAny		data();

	//	InputSlot		*m_Input;

	//};

}