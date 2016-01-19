#include "screamCore.h"


namespace SCore {

	void aabb3d::scale( float s )
	{
		vec3 middle = (bmin + bmax) * vec3(0.5);
		vec3 sz = (bmax - middle) * s;

		bmin = middle - sz;
		bmax = middle + sz;
	}

	void aabb3d::scaleForRotation()
	{
		vec3 middle = (bmin+bmax) * vec3(0.5);
		vec3 sz = (bmax - middle);

		float maxSz = glm::max( sz[0], glm::max( sz[1], sz[2] ) );
		float bboxDiagSz = glm::length( sz );

		bmin = middle - vec3( maxSz, maxSz, maxSz );
		bmax = middle + vec3( maxSz, maxSz, maxSz );

		scale( bboxDiagSz/maxSz );
	}


};
