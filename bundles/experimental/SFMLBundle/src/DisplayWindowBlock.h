#pragma once

#include "OpenGl.h"
#include "_2RealBlock.h"

class RessourceManagerBlock;

class DisplayWindowBlock : public _2Real::bundle::Block
{

public:

	DisplayWindowBlock( _2Real::bundle::ContextBlock &context );
	~DisplayWindowBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	sf::Clock							m_Clock;
	double								m_Count;
	RessourceManagerBlock				&m_Manager;
	_2Real::gl::RenderWindow			*m_Window;
	_2Real::gl::Renderer				*m_Renderer;
	_2Real::bundle::BlockHandle			m_BlockHandle;

};