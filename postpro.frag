
#version 120


varying vec2 uv;


uniform sampler2D scn;
uniform sampler2D blur;
uniform sampler2D glow;


uniform float sharpenfactor;
uniform float glowfactor;


vec4 lerp4( vec4 a, vec4 b, float t )
{
	return a + (b-a) * t;
}


void main()
{
	vec4 col = clamp( lerp4( texture2D( scn, uv ), texture2D( blur, uv ), sharpenfactor ), 0.0, 1.0 );
	gl_FragData[0] = max( col, texture2D( glow, uv ) * glowfactor );
}
