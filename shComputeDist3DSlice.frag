
#version 150 compatibility


varying vec2 uv;


uniform sampler2DArray texturePZ;
uniform sampler3D textureDistanceMapXY;


uniform float layer;
uniform float layerInvSize;
uniform float xInvSize, yInvSize;
uniform int depthPeelingLayers;


// the default GLSL sign function returns 3 values, with 0.0, which can cause bugs here.
float sign2( float x )
{
	return x >= 0.0 ? 1.0 : -1.0;
}


void updateMinVecDst( vec3 coords, inout vec3 minVec, inout float minDst )
{
	float dst = length( coords );
	if (dst < minDst)
	{
		minVec = coords;
		minDst = dst;
	}
}


float computeDistance3D( in vec3 uvw, inout vec3 minVec, in float inSignedDistance )
{
	float minDst = abs( inSignedDistance );
	float distanceSign = sign2( inSignedDistance );

	// ==============================================================================
	// z0 min
	const float biasz = -0.025;
	for (int i=0; i<depthPeelingLayers; i++)
	{
		vec2 inFragment = texture( texturePZ, vec3( vec2( 1.0 - uvw.x, uvw.y ), float(i) ) ).xy + vec2( biasz );
		inFragment.y = 1.0 - inFragment.y;

		if (inFragment.x > 0.001 + biasz) updateMinVecDst( vec3( 0.0, 0.0, inFragment.x - uvw.z ), minVec, minDst );
		if (inFragment.y < 0.999 + biasz) updateMinVecDst( vec3( 0.0, 0.0, inFragment.y - uvw.z ), minVec, minDst );
	}

	// ==============================================================================
	// xy scan along z
	float i;
	float zInvSize = layerInvSize;
	i = zInvSize;
	float tcz = uvw.z + i;
	while ( i < minDst && tcz < 0.999 )
	{
		vec2 minxy = texture( textureDistanceMapXY, vec3(uvw.xy, tcz) ).xy;
		updateMinVecDst( vec3( minxy, i ), minVec, minDst );
		i += zInvSize; tcz += zInvSize;
	}

	i = zInvSize;
	tcz = uvw.z - i;
	while ( i < minDst && tcz > 0.0 )
	{
		vec2 minxy = texture( textureDistanceMapXY, vec3(uvw.xy, tcz) ).xy;
		updateMinVecDst( vec3( minxy, -i ), minVec, minDst );
		i += zInvSize; tcz -= zInvSize;
	}

	// ==============================================================================

	return minDst * distanceSign;
}


void main()
{
	vec3 uvw = vec3( uv.xy , layer * layerInvSize );
	float signedDistance = texture( textureDistanceMapXY, uvw ).w;
	vec3 closestSurfaceVec = texture( textureDistanceMapXY, uvw ).xyz;

	signedDistance = computeDistance3D( uvw, closestSurfaceVec, signedDistance );

	gl_FragData[0] = vec4( closestSurfaceVec, signedDistance );
	//gl_FragData[0] = vec4( uvw, signedDistance );
}

