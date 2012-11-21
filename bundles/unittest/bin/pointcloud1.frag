#version 330 core

uniform sampler2D texRgb;
uniform sampler2D texUser;

in vec2 fTexcoord;

out vec4 fragColor;

void main()
{	
	vec4 userColor = texture( texUser, fTexcoord );
	vec4 rgbColor = texture( texRgb, fTexcoord );
	
	bool isUser = ( userColor.r > 0.0 || userColor.g > 0.0 || userColor.b > 0.0 );
	
	if ( isUser )
	fragColor = rgbColor;
	else
	fragColor = 0.5 * rgbColor + vec4( 0.5 );
}