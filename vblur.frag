

uniform float pixelUp;
varying vec2 uv;
uniform sampler2D texture;


void main()
{
	vec4 sum = vec4( 0.0 );
	
	for (int i=0; i<BLUR_ITER; i++)
	{
		sum += texture2D( texture, uv+vec2( 0.0, float(i)*pixelUp ) );
		sum += texture2D( texture, uv-vec2( 0.0, float(i)*pixelUp ) );
	}
	
	gl_FragData[0] = sum * INV_BLUR_ITER;
}
