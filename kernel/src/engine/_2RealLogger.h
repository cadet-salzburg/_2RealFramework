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

		typedef std::list< std::pair< long, std::string > >	LogList;

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

	inline Logger::Logger( std::string const& logfile ) :
		m_CurrLines( new LogList ),
		m_WriteLines( new LogList ),
		m_File( logfile ),
		m_KeepRunning( true ),
		m_StopEvent( true ),
		m_StartEvent( true ),
		m_Time()
	{
		if ( !m_File.is_open() )
		{
			throw NotFoundException("could not open logfile");
		}
		else
		{
			m_Thread.start( *this );
			m_StartEvent.wait();
			m_Time.update();
		}
	}

	inline Logger::~Logger()
	{
		m_File.close();

		delete m_CurrLines;
		delete m_WriteLines;
	}

	inline void Logger::addLine( std::string const& line )
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Access);

		m_CurrLines->push_back( std::make_pair( (long)m_Time.elapsed(), line ) );
	}

	inline void Logger::run()
	{
		m_StartEvent.set();

		while (1)
		{
			m_Access.lock();
			if ( m_KeepRunning )
			{
				std::swap( m_CurrLines, m_WriteLines );
				m_Access.unlock();
				for ( LogList::iterator it = m_WriteLines->begin(); it != m_WriteLines->end(); ++it )
				{
					m_File << it->first << " " << it->second << "\n";
					m_File.flush();
				}
				m_WriteLines->clear();
			}
			else
			{
				m_Access.unlock();
				break;
			}
		}

		m_StopEvent.set();
	}

	inline void Logger::stop()
	{
		{
			Poco::ScopedLock< Poco::FastMutex > lock(m_Access);
			m_KeepRunning = false;
		}

		m_StopEvent.wait();
	}

}