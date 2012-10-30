#pragma once

#include "_2RealBlock.h"
#include "Datatypes.h"

class RessourceManagerBlock : public _2Real::bundle::ContextBlock
{

public:

	RessourceManagerBlock();
	~RessourceManagerBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

	_2Real::gl::RessourceManager const& getManager() { return *mManager; }

private:

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::gl::RessourceManager		*mManager;

};