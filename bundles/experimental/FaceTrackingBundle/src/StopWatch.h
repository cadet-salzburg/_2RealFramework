#pragma once

namespace facetracking
{
	class TimeImpl;
}

class StopWatch
{
public:
	StopWatch();
	~StopWatch();

	void restart();
	double milliSeconds() const;

private:
	facetracking::TimeImpl *m_pImpl;
};