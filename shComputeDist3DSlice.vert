#version 150 compatibility

varying vec2 uv;

void main()
{
	uv = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}
