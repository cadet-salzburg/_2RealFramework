#version 330 core

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;

uniform sampler2D texUser;

in vec3 gPosition[];
in vec2 gTexcoord[];
 
out vec2 fTexcoord;

void main()
{
	bool validTriangle = true;
	if ( gPosition[ 0 ].z < 0.1 || gPosition[ 0 ].z > 5.0 ) validTriangle = false;
	if ( gPosition[ 1 ].z < 0.1 || gPosition[ 1 ].z > 5.0 ) validTriangle = false;
	if ( gPosition[ 2 ].z < 0.1 || gPosition[ 2 ].z > 5.0 ) validTriangle = false;

	vec3 v0 = gl_in[ 0 ].gl_Position.xyz - gl_in[ 1 ].gl_Position.xyz;
	vec3 v1 = gl_in[ 0 ].gl_Position.xyz - gl_in[ 2 ].gl_Position.xyz;
	vec3 v2 = gl_in[ 1 ].gl_Position.xyz - gl_in[ 2 ].gl_Position.xyz;
	float d0 = length( v0 );
	float d1 = length( v1 );
	float d2 = length( v2 );

	vec4 userColor0 = texture( texUser, gTexcoord[ 0 ] );
	vec4 userColor1 = texture( texUser, gTexcoord[ 1 ] );
	vec4 userColor2 = texture( texUser, gTexcoord[ 2 ] );	
	if ( !( userColor0.r > 0.0 || userColor0.g > 0.0 || userColor0.b > 0.0 ) ) validTriangle = false;
	if ( !( userColor1.r > 0.0 || userColor1.g > 0.0 || userColor1.b > 0.0 ) ) validTriangle = false;
	if ( !( userColor2.r > 0.0 || userColor2.g > 0.0 || userColor2.b > 0.0 ) ) validTriangle = false;
	
	if ( d0 > 0.3 ) validTriangle = false;
	if ( d1 > 0.3 ) validTriangle = false;
	if ( d2 > 0.3 ) validTriangle = false;

	if ( validTriangle )
	{
		fTexcoord = gTexcoord[ 0 ];
		gl_Position = gl_in[ 0 ].gl_Position;
		EmitVertex();

		fTexcoord = gTexcoord[ 1 ];
		gl_Position = gl_in[ 1 ].gl_Position;
		EmitVertex();

		fTexcoord = gTexcoord[ 2 ];
		gl_Position = gl_in[ 2 ].gl_Position;
		EmitVertex();

		EndPrimitive();
	}
}