#version 330 core

in vec3 position;
uniform mat3 matModel;
uniform mat4 matView;
uniform mat4 matProj;

void main()
{
	vec3 p = position;
	vec3 p1 = matModel * p;
	gl_Position = matProj * matView * vec4( p1, 1.0 );
}