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

#define ARRAY_BUFFER		0
#define INDEX_BUFFER		1
#define UPLOAD_BUFFER		2
#define DOWNLOAD_BUFFER		3

#define glewGetContext() this->getGlewContext().get()

namespace _2Real
{
	namespace gl
	{
		class TextureObj;
		class BufferObj;
		class ProgramObj;
		class ShaderObj;

		struct RenderSettings;
		class RenderWindow;
		class RenderContext;
		class Renderer;
		class OffscreenRenderer;

		typedef std::shared_ptr< GLEWContext >		GlewContext;
		typedef std::shared_ptr< TextureObj >		Texture;
		typedef std::shared_ptr< BufferObj >		Buffer;
		typedef std::shared_ptr< ShaderObj >		Shader;
		typedef std::shared_ptr< ProgramObj >		Program;
	}
}