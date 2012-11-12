#version 330 core

in vec3 position;
in vec2 texcoord;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

out vec3 gPosition;
out vec2 gTexcoord;

void main()
{
	gPosition = position;
	gTexcoord = texcoord;
	gl_Position = matProj * matView * matModel * vec4( position, 1.0 );
}