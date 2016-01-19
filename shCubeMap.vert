
#version 120

void main()
{
	gl_Position = ftransform();
	gl_TexCoord[0].xyz = gl_Vertex.xyz;
	//gl_FrontColor = gl_Color;
}
