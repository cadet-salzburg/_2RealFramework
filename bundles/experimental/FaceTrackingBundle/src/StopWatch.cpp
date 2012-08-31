#include "StopWatch.h"

#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"

namespace facetracking
{
	class TimeImpl
	{
	public:
		TimeImpl() :
			m_startTime()
		{}

		void restart()
		{
			m_startTime = Poco::Timestamp();
		}

		double milliSeconds() const
		{
			return ( m_startTime.elapsed() * 1e-6 );
		}

	private:
		Poco::Timestamp	m_startTime;
	};
}

StopWatch::StopWatch() :
	m_pImpl( new facetracking::TimeImpl() )
{}

StopWatch::~StopWatch()
{
	if( m_pImpl )
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}
}

void StopWatch::restart()
{
	m_pImpl->restart();
}

double StopWatch::milliSeconds() const
{
	return m_pImpl->milliSeconds();
}