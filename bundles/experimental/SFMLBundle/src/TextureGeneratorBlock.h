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

//class RenderDataGeneratorBlock : public _2Real::bundle::Block
//{
//
//public:
//
//	RenderDataGeneratorBlock( _2Real::bundle::ContextBlock &context );
//	~RenderDataGeneratorBlock();
//
//	void shutdown();
//	void update();
//	void setup( _2Real::bundle::BlockHandle &context );
//
//private:
//
//	RessourceManagerBlock				&mManager;
//
//	_2Real::bundle::BlockHandle			mBlockHandle;
//	_2Real::bundle::OutletHandle		mRenderDataOut;
//	_2Real::bundle::InletHandle			mAttributesIn;
//	_2Real::bundle::InletHandle			mTexturesIn;
//	_2Real::bundle::InletHandle			mVertexShaderIn;
//	_2Real::bundle::InletHandle			mFragmentShaderIn;
//	_2Real::bundle::InletHandle			mGeometryShaderIn;
//	_2Real::bundle::InletHandle			mUniformValuesIn;
//	_2Real::bundle::InletHandle			mAttributeBindingsIn;
//	_2Real::bundle::InletHandle			mFragDataBindingsIn;
//
//	gl::Program							mCurrentProgram;
//
//
//};