
#version 120


varying float depth;
varying vec2 uv;


void main()
{
	vec4 vertex = gl_ModelViewMatrix * gl_Vertex.zyxw;
	uv = (vertex.xy * 0.5) + vec2( 0.5 );
	depth = 1.0 - clamp((vertex.z * 0.5) + 0.5, 0.0, 1.0);

	gl_Position = vertex;
}
