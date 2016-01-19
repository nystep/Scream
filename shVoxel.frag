
#version 150 compatibility


in vec2 uv;
in vec3 eye;
in vec3 dir;
in vec3 dirDelta;


float deltapp1;


uniform samplerCube envmap;


// +-------------------------------------------------------------------------------------------------+
uniform vec3 sceneBBOXmin;
uniform vec3 sceneBBOXmax;


void intersectRayToBBox( in vec3 origin, in vec3 invDirection, out float nearT, out float farT )
{
	vec3 interT[2];

	interT[0] = (sceneBBOXmin - origin) * invDirection;
	interT[1] = (sceneBBOXmax - origin) * invDirection;

	vec3 signDir = max( sign( invDirection ), 0.0 );
	vec3 invSignDir = vec3( 1.0 ) - signDir;

	vec3 nt = signDir * interT[0] + invSignDir * interT[1];
	vec3 ft = invSignDir * interT[0] + signDir * interT[1];

	nearT = max( max( nt.x, nt.y ), nt.z );
	farT = min( min( ft.x, ft.y ), ft.z );
}

// +-------------------------------------------------------------------------------------------------+

uniform sampler3D sdmap;

void distanceFunc( vec3 p, out float closestMaterial, out float distance )
{
	vec3 uvw = clamp( fract( p ), 1.0/128.0, 1.0-1.0/128.0 );
	vec3 uvwpp = clamp( sign( p - round( p ) ) + uvw, 1.0/128.0, 1.0-1.0/128.0 );
	float mind = texture( sdmap, uvw ).w + length( p - vec3( clamp(p,sceneBBOXmin,sceneBBOXmax) ) );
	mind = min( texture( sdmap, vec3( uvwpp.x, uvw.y, uvw.z ) ).w + length( p - vec3( clamp(p,sceneBBOXmin,sceneBBOXmax) ) ), mind );
	mind = min( texture( sdmap, vec3( uvw.x, uvwpp.y, uvw.z ) ).w + length( p - vec3( clamp(p,sceneBBOXmin,sceneBBOXmax) ) ), mind );
	mind = min( texture( sdmap, vec3( uvw.x, uvw.y, uvwpp.z ) ).w + length( p - vec3( clamp(p,sceneBBOXmin,sceneBBOXmax) ) ), mind );
	distance = min( mind, p.y );
	closestMaterial = 1.0;
}

vec3 computeNormal( in vec3 p, in float epsilon )
{
	float dummy, eps7=epsilon*0.1;
	vec3 np, nm;

	vec3 epsx = vec3( epsilon, eps7, -eps7 );
	vec3 epsy = vec3( -eps7, epsilon, eps7 );
	vec3 epsz = vec3( eps7, -eps7, epsilon );
	
	distanceFunc( p+epsx, dummy, np.x );
	distanceFunc( p-epsx, dummy, nm.x );
	distanceFunc( p+epsy, dummy, np.y );
	distanceFunc( p-epsy, dummy, nm.y );
	distanceFunc( p+epsz, dummy, np.z );
	distanceFunc( p-epsz, dummy, nm.z );

	return normalize(vec3(np-nm));
}

vec4 coneMarching( in vec3 eye, in vec3 dir, in float tnear, in float tfar )
{
	float dist, epsilon;
	float t = tnear;
	float closestMat = 0.0;

	for (int i=0; i<1000; i++)
	{
		if (t >= tfar)
		{
			distanceFunc( eye+dir*tfar, closestMat, dist );
			if (dist > 0.0) closestMat = 0.0;
			break;
		}
		epsilon = t * deltapp1;
		distanceFunc( eye+dir*t, closestMat, dist );
		if (dist < epsilon) break;
		t += dist;
	}
	
	if (closestMat == 0.0)
		return texture( envmap, dir );
	else
	{
		vec3 pos = eye+dir*t;
		float lamb = clamp( dot( computeNormal( pos, 1.0/48.0 ), normalize( vec3(1.0) ) ), 0.0, 1.0 );
		return vec4( lamb );
	}
}

// +-------------------------------------------------------------------------------------------------+
void main()
{
	float tnear, tfar;
	vec3 direction = normalize( dir ), neighbourdirection = normalize( dirDelta );
	vec3 invDir = vec3( 1.0 ) / direction;

	deltapp1 = acos(dot(direction, neighbourdirection));

	intersectRayToBBox( eye, invDir, tnear, tfar );
	tnear = max( tnear , 0.0 );
	tfar = max( tfar , 0.0 );

	if (tnear>=tfar)
		gl_FragData[0] = texture( envmap, direction );
	else
		gl_FragData[0] = coneMarching( eye, direction, tnear, tfar );
}
