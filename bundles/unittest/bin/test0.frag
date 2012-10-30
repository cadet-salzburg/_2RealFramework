#version 330 core

uniform sampler2D texRgb;

in vec2 fTexcoord;

out vec4 fragColor;

void main()
{	
	fragColor = texture( texRgb, fTexcoord );
}