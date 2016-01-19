
#version 120
#extension ARB_draw_buffers : require

uniform samplerCube tu0;


void main()
{
	vec4 color = textureCube( tu0, normalize( gl_TexCoord[0].xyz ) );
	gl_FragData[0] = color;
	gl_FragData[1] = vec4( pow( vec3(0.5) * (vec3(1.0) + sin( color.rgb * vec3(3.1415926) - vec3(1.57079632679) ) ), vec3(2.5) ), color.a );
}
