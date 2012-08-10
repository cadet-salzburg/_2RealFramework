#pragma once

#include "GL\glew.h"

#include "_2RealBlock.h"
#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

class WindowBlock : public _2Real::bundle::Block
{

public:

	WindowBlock();
	~WindowBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

	GLEWContext * getGlewContext() const;

private:

	sf::Window							*m_Win;
	_2Real::bundle::BlockHandle			m_Block;
	GLuint								m_Texture;
	GLEWContext							*m_GlewContext;

};