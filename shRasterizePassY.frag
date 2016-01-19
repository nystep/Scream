
#version 120


varying float depth;
varying vec2 uv;

uniform bool lookUpPrev;
uniform sampler2D prevDp;


void main()
{
	float d = gl_FrontFacing ? depth : 0.0;
	float id = gl_FrontFacing ? 0.0 : 1.0 - depth;

	if (lookUpPrev)
	{
		vec2 prevFragment = texture2D( prevDp, uv ).xy - vec2( 0.0005 );
		
		if (d >= prevFragment.x) d = 0.0;
		if (id >= prevFragment.y) id = 0.0;
		
		gl_FragData[0].xy = vec2( d, id );
	}
	else
		gl_FragData[0].xy = vec2( d, id );
}
