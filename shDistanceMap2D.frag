#version 150 compatibility


varying vec2 uv;


uniform sampler2DArray texturePX;
uniform sampler2DArray texturePY;
uniform sampler2DArray texturePZ;


uniform float layer;
uniform float layerInvSize;
uniform int depthPeelingLayers;
uniform float xInvSize, yInvSize;


float sign2( float x )
{
	return x >= 0.0 ? 1.0 : -1.0;
}


void updateMinVecDst( vec3 coords, inout vec3 minVec, inout float minDst )
{
	float dst = length(coords);
	if (dst < minDst)
	{
		minVec = coords;
		minDst = dst;
	}
}


vec4 computeDistance2D( vec3 uvw )
{
	float distanceSign = 0.0;
	vec3 minVec = vec3( -1.0 );
	float minDst = length( minVec );

	// init...
	float CtrPX = 0.0;
	float CtrNX = 0.0;
	for (int i=0; i<depthPeelingLayers; i++)
	{
		vec2 inFragment = texture( texturePX, vec3( uvw.zy, float(i) ) ).xy;
		inFragment.y = 1.0 - inFragment.y;

		if (inFragment.x > 0.001)
		{
			updateMinVecDst( vec3( inFragment.x - uvw.x, 0.0, 0.0 ), minVec, minDst );
			if (inFragment.x - uvw.x > 0.0) CtrNX -= 1.0; else CtrPX += 1.0;
		}
		if (inFragment.y < 0.999)
		{
			updateMinVecDst( vec3( inFragment.y - uvw.x, 0.0, 0.0 ), minVec, minDst );
			if (inFragment.y - uvw.x > 0.0) CtrNX += 1.0; else CtrPX -= 1.0;
		}
	}
	distanceSign += sign(CtrPX);
	distanceSign += sign(CtrNX);

	float CtrPY = 0.0;
	float CtrNY = 0.0;
	for (int i=0; i<depthPeelingLayers; i++)
	{
		vec2 inFragment = texture( texturePY, vec3( vec2( 1.0 - uvw.x, uvw.z ), float(i) ) ).xy;
		inFragment.x = 1.0 - inFragment.x;

		if (inFragment.x < 0.999)
		{
			updateMinVecDst( vec3( 0.0, inFragment.x - uvw.y, 0.0 ), minVec, minDst );
			if (inFragment.x - uvw.y > 0.0) CtrNY += 1.0; else CtrPY -= 1.0;
		}
		if (inFragment.y > 0.001)
		{
			updateMinVecDst( vec3( 0.0, inFragment.y - uvw.y, 0.0 ), minVec, minDst );
			if (inFragment.y - uvw.y > 0.0) CtrNY -= 1.0; else CtrPY += 1.0;
		}
	}
	distanceSign += sign(CtrPY);
	distanceSign += sign(CtrNY);

	const float biasz = -0.025;
	float CtrPZ = 0.0;
	float CtrNZ = 0.0;
	for (int i=0; i<depthPeelingLayers; i++)
	{
		vec2 inFragment = texture( texturePZ, vec3( vec2( 1.0 - uvw.x, uvw.y ), float(i) ) ).xy + vec2( biasz );
		inFragment.y = 1.0 - inFragment.y;

		if (inFragment.x > 0.001 + biasz)
		{
			//updateMinVecDst( vec3( 0.0, 0.0, inFragment.x - uvw.z ), minVec, minDst );
			if (inFragment.x - uvw.z > 0.0) CtrNZ -= 1.0; else CtrPZ += 1.0;
		}
		if (inFragment.y < 0.999 + biasz)
		{
			//updateMinVecDst( vec3( 0.0, 0.0, inFragment.y - uvw.z ), minVec, minDst );
			if (inFragment.y - uvw.z > 0.0) CtrNZ += 1.0; else CtrPZ -= 1.0;
		}
	}
	distanceSign += sign(CtrPZ);
	distanceSign += sign(CtrNZ);

	// +------------------------------------------------------------------------------------------+
	// scan in X direction
	float i = yInvSize, tcy = uvw.y + i;
	while ( i < minDst && tcy < 0.999 )
	{
		for (int j=0; j<depthPeelingLayers; j++)
		{
			vec2 inFragment = texture( texturePX, vec3( uvw.z, tcy, float(j) ) ).xy;
			inFragment.y = 1.0 - inFragment.y;

			if (inFragment.x > 0.001) updateMinVecDst( vec3( inFragment.x - uvw.x, i, 0.0 ), minVec, minDst );
			if (inFragment.y < 0.999) updateMinVecDst( vec3( inFragment.y - uvw.x, i, 0.0 ), minVec, minDst );
		}

		i += yInvSize; tcy += yInvSize;
	}

	i = yInvSize;
	tcy = uvw.y - i;
	while ( i < minDst && tcy > 0.0 )
	{
		for (int j=0; j<depthPeelingLayers; j++)
		{
			vec2 inFragment = texture( texturePX, vec3( uvw.z, tcy, float(j) ) ).xy;
			inFragment.y = 1.0 - inFragment.y;

			if (inFragment.x > 0.001) updateMinVecDst( vec3( inFragment.x - uvw.x, -i, 0.0 ), minVec, minDst );
			if (inFragment.y < 0.999) updateMinVecDst( vec3( inFragment.y - uvw.x, -i, 0.0 ), minVec, minDst );
		}

		i += yInvSize; tcy -= yInvSize;
	}

	// +------------------------------------------------------------------------------------------+
	// scan in Y direction
	i = xInvSize;
	float tcx = 1.0 - (uvw.x + i);
	while ( i < minDst && tcx > 0.0 )
	{
		for (int j=0; j<depthPeelingLayers; j++)
		{
			vec2 inFragment = texture( texturePY, vec3( tcx, uvw.z, float(j) ) ).xy;
			inFragment.x = 1.0 - inFragment.x;

			if (inFragment.y > 0.001) updateMinVecDst( vec3( -i, inFragment.y - uvw.y, 0.0 ), minVec, minDst );
			if (inFragment.x < 0.999) updateMinVecDst( vec3( -i, inFragment.x - uvw.y, 0.0 ), minVec, minDst );
		}

		i += xInvSize; tcx -= xInvSize;
	}

	i = xInvSize;
	tcx = 1.0 - (uvw.x - i);
	while ( i < minDst && tcx < 0.999 )
	{
		for (int j=0; j<depthPeelingLayers; j++)
		{
			vec2 inFragment = texture( texturePY, vec3( tcx, uvw.z, float(j) ) ).xy;
			inFragment.x = 1.0 - inFragment.x;

			if (inFragment.y > 0.001) updateMinVecDst( vec3( i, inFragment.y - uvw.y, 0.0 ), minVec, minDst );
			if (inFragment.x < 0.999) updateMinVecDst( vec3( i, inFragment.x - uvw.y, 0.0 ), minVec, minDst );
		}

		i += xInvSize; tcx += xInvSize;
	}

	return vec4( minVec, minDst * sign2(distanceSign + 2.0) );
}


void main()
{
	gl_FragData[0] = computeDistance2D( vec3( uv.xy , layer * layerInvSize ) );
}

