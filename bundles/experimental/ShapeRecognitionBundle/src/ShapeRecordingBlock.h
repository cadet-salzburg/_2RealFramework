#pragma once

#include "_2RealBlock.h"

class ShapeRecordingBlockImpl;

class ShapeRecordingBlock : public _2Real::bundle::Block
{

public:
	ShapeRecordingBlock();
	~ShapeRecordingBlock();
	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::BlockHandle			m_Block;

	_2Real::bundle::InletHandle			m_depthImageIn;
	_2Real::bundle::InletHandle			m_outputPathIn;
	_2Real::bundle::InletHandle			m_saveIn;

	ShapeRecordingBlockImpl	*m_blockImpl;
};