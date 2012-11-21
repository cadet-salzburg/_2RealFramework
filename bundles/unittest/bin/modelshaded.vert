#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

out vec3 eyePosition;
out vec3 eyeNormal;

void main()
{
	eyePosition = ( matView * matModel * vec4( position, 1.0 ) ).xyz;
	eyeNormal = ( matView * matModel * vec4( normal, 0.0 ) ).xyz;

	gl_Position = matProj * matView * matModel * vec4( position, 1.0 );
}