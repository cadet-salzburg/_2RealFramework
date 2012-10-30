#version 330 core

in vec2 position;
in vec2 texcoord;

uniform sampler2D texDepth;

uniform mat4 matModel;

out vec2 gTexcoord;
out vec3 gPosition;

void main()
{
	float depth = texture( texDepth, texcoord ).r;
	//depth = 1.0 - depth;
	
	gTexcoord = texcoord;
	gPosition = vec3( position, 0.0 );
	
	gl_Position = matModel * vec4( position, depth, 1.0 );
}