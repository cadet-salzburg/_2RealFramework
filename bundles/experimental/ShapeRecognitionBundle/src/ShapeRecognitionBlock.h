#pragma once

#include "_2RealBlock.h"

class ShapeRecognitionBlockImpl;


class ShapeRecognitionBlock : public _2Real::bundle::Block
{

public:
	ShapeRecognitionBlock();
	~ShapeRecognitionBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::BlockHandle			m_Block;

	_2Real::bundle::InletHandle			m_dataPathIn;
	_2Real::bundle::InletHandle			m_depthImageIn;

	_2Real::bundle::OutletHandle		m_fileNameOut;
	
	ShapeRecognitionBlockImpl	*m_blockImpl;
};