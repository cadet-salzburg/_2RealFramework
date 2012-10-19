#pragma once

#include "OpenGl.h"
#include "_2RealBlock.h"

class RessourceManagerBlock;

class RandomTexture2DBlock : public _2Real::bundle::Block
{

public:

	RandomTexture2DBlock( _2Real::bundle::ContextBlock &context );
	~RandomTexture2DBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&m_Manager;
	_2Real::bundle::BlockHandle			m_BlockHandle;
	_2Real::gl::RenderContext			*m_Context;
	_2Real::gl::Renderer				*m_Renderer;

	_2Real::gl::RenderData::TextureBinding	m_Texture;
	_2Real::gl::RenderData::VertexAttribute	m_Positions;
	_2Real::gl::RenderData::VertexAttribute	m_TexCoords;

};

class ImageToTexture2DBlock : public _2Real::bundle::Block
{

public:

	ImageToTexture2DBlock( _2Real::bundle::ContextBlock &context );
	~ImageToTexture2DBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&m_Manager;
	_2Real::bundle::BlockHandle			m_BlockHandle;
	_2Real::gl::RenderContext			*m_Context;
	_2Real::gl::Renderer				*m_Renderer;

	_2Real::gl::RenderData::TextureBinding	m_Texture;
	_2Real::gl::RenderData::VertexAttribute	m_Positions;
	_2Real::gl::RenderData::VertexAttribute	m_TexCoords;

};