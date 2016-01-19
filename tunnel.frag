varying vec3 u,e,o;
uniform sampler2D sa;
float t,a,c;

vec3 zx(vec3 r,float v)
{
	return vec3(r.x*cos(v)+r.z*sin(v),r.y,r.z*cos(v)-r.x*sin(v));
}

vec3 zy(vec3 r,float v)
{
	return vec3(r.y*cos(v)+r.z*sin(v),r.x,r.z*cos(v)-r.y*sin(v));
}

vec2 xd(vec3 p)
{
	return vec2(50.0-length(p.xz),0.0);
}

vec2 yd(vec3 p)
{
	vec3 v=vec3(atan(p.z,p.x)*0.5/3.1416,p.y*0.1,60.0-length(p.xz)),c=v;
	float r2,r=3.0;
	float fR2=1.0;
	float mR2=0.25;

	for (int i=0;i<5;i++)
	{
		v=sign(v)*(1.0-abs(abs(v)-1.0));
		r2=length(v);

		if(r2<mR2) {v*=fR2/mR2;r*=fR2/mR2;}
		else if(r2<fR2) {v*=fR2/r2;r*=fR2/r2;}

		v=v*3.0+c;
		r=r*3.0+1.0;
	}

	return vec2((length(v)-2.0)/abs(r)-pow(3.0,1-i),0.0);
}

vec3 n(vec3 p)
{
	float f=yd(p).x;
	vec2 e=vec2(a,1.0);
	return normalize(vec3(f-yd(p+e.xyy).x,f-yd(p+e.yxy).x,f-yd(p+e.yyx).x));
}

vec3 m(vec3 p)
{
	vec2 v=vec2(atan(p.z,p.x)*0.5/3.1416,p.y*0.25);
	return texture2D(sa,v).w*vec3(0.8,0.6,0.4)+0.25;
}

void main()
{
	vec2 r;
	vec3 p,i=normalize(zy(zx(vec3(u.xy,1.0),o.x),o.y));
	t=1.0,c=0.0;
	while(t+c<100.0)
	{
		a=t*0.001;
		p=e+i*t;
		r=xd(p);
		if(r.x<a){break;}
		t+=r.x;
		c+=1.0;
	}
	while(t+c<100.0)
	{
		a=t*0.001;
		p=e+i*t;
		r=yd(p);
		if(r.x<a){break;}
		t+=r.x;
		c+=1.0;
	}
	gl_FragColor=vec4(c*0.001);
}
