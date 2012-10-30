#version 330 core

uniform sampler2D texRgb;
uniform sampler2D texDepth;

in vec2 fTexcoord;
in vec3 fPosition;

out vec4 fragColor;

void main()
{	
	//const float delta = 0.01;
	//const vec3 light = vec3( 0.0, 0.0, 1.0 );
	
	//vec2 tx0 = fTexcoord + vec2( delta, 0.0 );
	//vec2 tx1 = fTexcoord + vec2( -delta, 0.0 );
	//vec2 ty0 = fTexcoord + vec2( 0.0, delta );
	//vec2 ty1 = fTexcoord + vec2( 0.0, -delta );
	
	//const float depth = 1.0 - texture( texDepth, fTexcoord ).r;
	//float gx0 = texture( texDepth, tx0 ).r;
    //float gx1 = texture( texDepth, tx1 ).r;
    //float gy0 = texture( texDepth, ty0 ).r;
    //float gy1 = texture( texDepth, ty1 ).r;
	
    //vec3 va = normalize( vec3( 2.0, 0.0, gx1-gx0 ) );
	//vec3 vb = normalize( vec3( 0.0, 2.0, gy1-gy0 ) );	
	//vec3 normal = cross( va, vb );
	//normal.z = 0.0;	
	//vec3 N = normalize( normal );
	//vec3 L = normalize( light - fPosition );	
	//float lambert = dot( N, L );
	
	//fragColor = vec4( vec3( lambert ), 1.0 );
	float equalizedDepth = 1.0-texture( texRgb, fTexcoord ).r;
	fragColor = vec4( vec3( equalizedDepth ), 1.0 );
}