varying vec3 u,e,o;
void main()
{
	u.xy=gl_Vertex.xy*vec2(1.5,1.0);
	u.z=u.x*.1;
	e=vec3(sin(u.z)*cos(u.z*0.7+1.0),-u.z,cos(u.z)*sin(u.z*0.7-1.0))*0.5;
	o=vec3(sin(u.z)*sin(u.z),-sin(u.z),cos(u.z+1.0));
	gl_Position=gl_Vertex;
}
