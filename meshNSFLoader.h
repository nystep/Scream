#ifndef MESHNSFLOADER_H_INCLUDED
#define MESHNSFLOADER_H_INCLUDED


namespace SCore {

	class nsfMesh {
	public:

		aabb3d boundingBox;

		unsigned int materialIdx;

		unsigned int numTriangles;
		meshVBO *vboTriangleIndices;
		//unsigned int *triangleIndices;

		unsigned int numVertex;
		int idxVertex;
		int idxNormals;
		int idxTangentSpace;
		meshVBO *vboVertexData;
		meshAttributeSet *vboVertexAttributes;

		unsigned int numBones;

		unsigned char numUVComponents[4];
		int idxUvw[4];

		nsfMesh() : boundingBox(), materialIdx( 0 ), numTriangles( 0 ),
			vboTriangleIndices( 0 ), numVertex( 0 ), idxVertex( 0 ),
			vboVertexData( 0 ), vboVertexAttributes( 0 ), numBones( 0 )
		{
			idxNormals = idxTangentSpace = 0;
			idxUvw[0] = idxUvw[1] = idxUvw[2] = idxUvw[3] = 0;
			numUVComponents[0] = numUVComponents[1] = numUVComponents[2] = numUVComponents[3] = 0;
		}

	};

	typedef struct {
		unsigned int type, semantic, index, dataLength;
		char *name;
		char *data;
	} nsfMaterialProperty;

	class nsfMaterial
	{
	public:
		unsigned int randomMaterialColor;
		std::vector<nsfMaterialProperty> properties;

		nsfMaterial() : randomMaterialColor(0), properties() {}
	};

	typedef struct {
	} nsfLight;

	typedef struct {
	} nsfCamera;

	typedef struct {
	} nsfAnimation;

	typedef struct {
	} nsfNode;

	class nsfScene {
	public:
		unsigned int numMeshes;
		nsfMesh *meshes;
		unsigned int numMaterials;
		nsfMaterial *materials;
		unsigned int numLights;
		nsfLight *lights;
		unsigned int numCameras;
		nsfCamera *cameras;
		unsigned int numAnimations;
		nsfAnimation *animations;
		unsigned int numNodes;
		nsfNode *nodes;

		nsfScene() : numMeshes(0), meshes(0), numMaterials(0), materials(0), numLights(0), lights(0),
			numCameras(0), cameras(0), numAnimations(0), animations(0), numNodes(0), nodes(0)
		{}

		void render( shader *s, int nLayers=1 );
		void renderMesh( shader *s, int meshNum=0, int nLayers=1 );

	};

	nsfScene* meshNSFLoader( const char* filename );

}

#endif // MESHNSFLOADER_H_INCLUDED
