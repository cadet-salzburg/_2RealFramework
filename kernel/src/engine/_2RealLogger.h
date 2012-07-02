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

#include "helpers/_2RealPoco.h"

#include <list>
#include <fstream>

namespace _2Real
{
	class Logger : public Poco::Runnable
	{

	public:

		Logger( std::string const& logfile );
		~Logger();

		void run();
		void addLine( std::string const& line );
		void stop();

	private:

		typedef std::list< std::pair< long, std::string > >					LogList;
		typedef std::list< std::pair< long, std::string > >::iterator		LogListIterator;
		typedef std::list< std::pair< long, std::string > >::const_iterator	LogListConstIterator;

		mutable Poco::FastMutex		m_Access;
		LogList						*m_CurrLines;
		LogList						*m_WriteLines;
		std::ofstream				m_File;
		Poco::Thread				m_Thread;
		bool						m_KeepRunning;
		Poco::Event					m_StopEvent;
		Poco::Event					m_StartEvent;
		Poco::Timestamp				m_Time;

	};
}