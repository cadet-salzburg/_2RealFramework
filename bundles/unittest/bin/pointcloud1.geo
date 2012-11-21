#version 330 core

layout( points ) in;
layout( points, max_vertices=3 ) out;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

uniform sampler2D texUser;

in vec3 gPosition[];
in vec2 gTexcoord[];
 
out vec2 fTexcoord;

void main()
{
	vec4 userColor0 = texture( texUser, gTexcoord[ 0 ] );
	//if ( userColor0.r > 0.0 || userColor0.g > 0.0 || userColor0.b > 0.0 )
	//{
		fTexcoord = gTexcoord[ 0 ];
		gl_Position = gl_in[ 0 ].gl_Position;
		EmitVertex();
	//}
}