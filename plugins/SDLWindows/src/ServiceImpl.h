#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

class SDLManager : public _2Real::Block
{

public:

	SDLManager();
	~SDLManager();
	void update();
	void shutdown();
	void setup( _2Real::FrameworkContext &context );

private:

};

class ImageDisplay : public _2Real::Block
{

public:

	ImageDisplay() : Block() {}
	~ImageDisplay() {}
	void shutdown() {}
	void update();
	void setup( _2Real::FrameworkContext &context );

private:

};