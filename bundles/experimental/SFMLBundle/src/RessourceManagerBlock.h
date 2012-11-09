#pragma once

#include "_2RealBlock.h"

namespace _2Real
{
	namespace gl
	{
		struct RenderSettings;
		class RessourceManager;
	}
}

class RessourceManagerBlock : public _2Real::bundle::ContextBlock
{

public:

	RessourceManagerBlock();
	~RessourceManagerBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

	_2Real::gl::RessourceManager const& getManager() const;
	_2Real::gl::RenderSettings const& getRenderSettings() const;

private:

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::gl::RessourceManager		*mManager;

};