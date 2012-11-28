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

#define GLEW_MX

#include "GL/glew.h"

#include "Poco/Mutex.h"

#include <memory.h>
#include <vector>
#include <list>
#include <map>
#include <assert.h>
#include <string>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#define glewGetContext() this->getGlewContext().get()

namespace _2Real
{
	namespace gl
	{
		struct TextureObj;
		struct BufferObj;
		struct ProgramObj;
		struct ShaderObj;
		struct RenderData;

		struct RenderSettings
		{
			std::string		title;
			unsigned int	width;
			unsigned int	height;
			unsigned int	glMajor;
			unsigned int	glMinor;
			unsigned int	depthBits;
			unsigned int	stencilBits;
			unsigned int	aaSamples;
			unsigned int	colorBits;
		};

		class RessourceManager;
		class RenderWindow;
		class Renderer;
		class Context;

		typedef std::shared_ptr< GLEWContext >		GlewContext;
		typedef std::shared_ptr< const TextureObj >	Texture;
		typedef std::shared_ptr< const BufferObj >	Buffer;
		typedef std::shared_ptr< const ProgramObj >	Program;
		typedef std::shared_ptr< const ShaderObj >	Shader;
	}
}