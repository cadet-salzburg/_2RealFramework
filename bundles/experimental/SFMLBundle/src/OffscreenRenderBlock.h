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
//#pragma once
//
//#include "OpenGl.h"
//#include "_2RealBlock.h"
//
//class RessourceManagerBlock;
//
//class OffscreenRenderBlock : public _2Real::bundle::Block
//{
//
//public:
//
//	OffscreenRenderBlock( _2Real::bundle::ContextBlock &context );
//	~OffscreenRenderBlock();
//
//	void shutdown();
//	void update();
//	void setup( _2Real::bundle::BlockHandle &context );
//
//protected:
//
//	RessourceManagerBlock				&m_Manager;
//	_2Real::gl::RenderContext			*m_Context;
//	_2Real::gl::OffScreenRenderer		*m_Renderer;
//	_2Real::bundle::BlockHandle			m_Block;
//
//	_2Real::gl::RenderData::TextureBinding	m_Texture;
//	_2Real::gl::RenderData::VertexAttribute	m_Positions;
//	_2Real::gl::RenderData::VertexAttribute	m_TexCoords;
//
//};
//
//class ImageSubtractionBlock : public OffscreenRenderBlock
//{
//
//public:
//
//	ImageSubtractionBlock( _2Real::bundle::ContextBlock &context );
//	~ImageSubtractionBlock();
//
//	void shutdown();
//	void update();
//	void setup( _2Real::bundle::BlockHandle &context );
//
//private:
//
//	_2Real::gl::RenderData::VertexAttribute	m_PointPositions;
//	_2Real::gl::RenderData::VertexAttribute	m_PointTexcoords;
//
//};