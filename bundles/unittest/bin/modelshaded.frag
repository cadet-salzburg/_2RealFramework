#version 330 core

out vec4 fragColor;

in vec3 eyePosition;
in vec3 eyeNormal;

const float shininess = 20.0;

void main()
{	
	float ambient = 0.05;
	
	vec3 N = normalize( eyeNormal );
	if ( !gl_FrontFacing ) N *= -1.0;
	vec3 L = normalize( vec3( -1.0, -1.0, -1.0 ) );
	vec3 R = reflect( L, N );
	vec3 V = normalize( -eyePosition );
	
	float diffuse = max( dot( N, L ), 0.0 );
	
	float specular = 0.0;
	if ( diffuse > 0.0 )
	{
		specular = pow( max( dot( R, V ), 0.0 ), shininess );
	}
	
	float final = 0.1*ambient + 0.6*diffuse + 0.3*specular;
	
	fragColor = vec4( vec3( final ), 1.0 );
}