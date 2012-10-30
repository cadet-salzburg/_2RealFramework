#version 330 core

layout( points ) in;
layout( points ) out;

uniform sampler2D texUser;

in vec2 gTexcoord[];
in vec3 gPosition[];

out vec2 fTexcoord;
out vec3 fPosition;

void main()
{
	vec4 userColor = texture( texUser, gTexcoord[ 0 ] );
	float lum = userColor.x + userColor.y + userColor.z;
	
	if( lum > 0.5 )
	{
		fTexcoord = gTexcoord[ 0 ];
		fPosition = gPosition[ 0 ];
		gl_Position = gl_in[ 0 ].gl_Position;
		EmitVertex();
	}
}