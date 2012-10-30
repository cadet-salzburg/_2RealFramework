#pragma once

#define GLEW_MX

#include "GL/glew.h"

#include "_2RealDatatypes.h"

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

		struct RenderSettings;
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