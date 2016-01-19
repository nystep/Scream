
varying vec2 uv;

uniform sampler2D texture;

void main()
{
	gl_FragData[0] = texture2D( texture, uv );
}
