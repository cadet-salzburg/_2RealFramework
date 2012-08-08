#pragma once

#include <iostream>

//don't panic, i'll remove boost dependencies in final version, this is just for debugging
//TODO: remove bench class
#include <boost/timer/timer.hpp>

class Bench
{
private:
	bool paused;
	bool running;

	double akku;

	const std::string	name;

	std::ostream	&out;

	boost::timer::cpu_timer timer;

	Bench( const Bench& );	//not implemented, intentionally

public:
	explicit Bench( const std::string &name, bool startPaused = false ) :
		paused( startPaused ),
		running( true ),
		akku( 0.0 ),
		name( name ),
		out( std::cout )
	{
	}

	Bench( const std::string &name, std::ostream &out, bool startPaused = false ) :
		paused( startPaused ),
		running( true ),
		akku( 0.0 ),
		name( name ),
		out( out )
	{
	}

	~Bench()
	{
		if( this->running )
			this->stop();
	}

	void reset( bool pause = false )
	{
		this->paused = pause;
		this->running = true;
		this->akku = 0.0;
		this->timer.start();
	}

	void pause()
	{
		if( !this->paused )
		{
			this->paused = true;

			if( this->running )
				this->akku += timer.elapsed().wall * 1e-6f;
		}
	}

	void resume()
	{
		if( this->running && this->paused )
		{
			this->paused = false;
			this->timer.start();
		}
	}

	void stop( bool restart = false, bool restartPaused = false )
	{
		if( this->running )
		{
			double dt = this->akku;
			if( !this->paused )
				dt += timer.elapsed().wall * 1e-6f;
			this->out << name << " took " << dt << "ms" << std::endl;

			if( restart )
				this->reset( restartPaused );
			else
				this->running = false;
		}
	}
};