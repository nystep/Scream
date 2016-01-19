
vec4 c(vec4 x)
{
	x-=fract(x);
	return fract((x*0.01172+vec4(0.97))*(x*0.05911+vec4(0.91))*(x*0.07352+vec4(0.95)));
}

float n3(vec3 p)
{
	vec3 rp=round(p);
	vec3 u=smoothstep(0.0,1.0,p-rp);
	vec3 v=vec3(1.0,59.0,151.0);
    vec4 n=vec4(11.0+dot(rp,v))+vec4(0.0,v.x,v.y,v.x+v.y);
	vec4 m=mix(c(n),c(n+vec4(v.z)),u.z);
	vec2 n=mix(m.xy,m.zw,u.y);
	return mix(n.x,n.y,u.x);
}

void main()
{
	gl_FragColor=c(gl_FragCoord);
}
