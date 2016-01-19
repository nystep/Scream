

uniform float pixelRight;
varying vec2 uv;
uniform sampler2D texture;


void main()
{
	vec4 sum = vec4( 0.0 );
	
	for (int i=-BLUR_ITER; i<=BLUR_ITER; i++)
	{
		sum += texture2D( texture, uv+vec2( float(i)*pixelRight, 0.0 ) );
	}
	
	gl_FragData[0] = sum * INV_BLUR_ITER;
}

