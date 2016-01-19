
#version 150 compatibility

out vec2 uv;
out vec3 eye;
out vec3 dir;
out vec3 dirDelta;

uniform float stepAngle;


void main()
{
	eye = -gl_ModelViewMatrixInverse[3].xyz;
	dir = - (gl_ModelViewMatrixInverse[2].xyz - 
			 gl_ModelViewMatrixInverse[1].xyz * gl_Vertex.y * 0.7 - 
			 gl_ModelViewMatrixInverse[0].xyz * gl_Vertex.x);
	dirDelta = - (gl_ModelViewMatrixInverse[2].xyz - 
				  gl_ModelViewMatrixInverse[1].xyz * (gl_Vertex.y + stepAngle) * 0.7 - 
				  gl_ModelViewMatrixInverse[0].xyz * (gl_Vertex.x + stepAngle));

	uv = gl_MultiTexCoord0.xy;
	gl_Position = vec4( gl_Vertex.x, gl_Vertex.y, 0.0, 1.0 );
}
