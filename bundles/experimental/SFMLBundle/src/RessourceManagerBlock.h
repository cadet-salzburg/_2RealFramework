#pragma once

#include "OpenGl.h"
#include "_2RealBlock.h"

#include "Poco/Mutex.h"
#include "Poco/Event.h"

#include <deque>

class RessourceManagerBlock : public _2Real::bundle::ContextBlock
{

public:

	RessourceManagerBlock();
	~RessourceManagerBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

	_2Real::gl::Texture createTexture( const GLenum target );
	_2Real::gl::Buffer createBuffer( const GLenum usage );

private:

	struct Tex
	{
		_2Real::gl::Texture		texture;
		GLenum					target;
		Poco::Event				creation;
	};

	struct Buf
	{
		_2Real::gl::Buffer		buffer;
		GLenum					usage;
		Poco::Event				creation;
	};

	_2Real::bundle::BlockHandle				m_Block;
	_2Real::gl::GlRessourceManager			*m_RessourceManager;

	Poco::FastMutex							m_Mutex;
	std::deque< Tex* >						m_Textures;
	std::deque< Buf* >						m_Buffers;

};