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