
#include "screamCore.h"
#include <stdlib.h>
#include <cstdio>

namespace SCore {

	static void importMeshes( iobFile *infile, DataBuffer* databuff, nsfScene *scene )
	{
		unsigned short nMeshes;
		infile->readUS( nMeshes );
		scene->numMeshes = nMeshes;
		scene->meshes = new nsfMesh [nMeshes];

		for (int i=0; i<nMeshes; i++)
		{
			infile->readUI( scene->meshes[i].numTriangles );
			infile->readUI( scene->meshes[i].numVertex );
			infile->readUI( scene->meshes[i].numBones );
			infile->readUI( scene->meshes[i].materialIdx );

			/*
			printf( "mesh %u: %u vertex, %u triangles, %u bones, material %u\n", i,
			scene->meshes[i].numVertex, scene->meshes[i].numTriangles, scene->meshes[i].numBones, scene->meshes[i].materialIdx );
			*/

			// read triangle indice data

			unsigned int stri0 = databuff->readUintArray( infile );

			//databuff->deltaUnpackUintArray( stri0 );

			unsigned int stri1 = databuff->readUintArray( infile );
			unsigned int stri2 = databuff->readUintArray( infile );

			scene->meshes[i].vboTriangleIndices = new meshVBO;

			unsigned int *temporaryIndices = new unsigned int [scene->meshes[i].numTriangles*3];

			for (unsigned int j=0; j<scene->meshes[i].numTriangles; j++)
			{
				temporaryIndices[j*3] = databuff->getUint( stri0, j );
				temporaryIndices[j*3+1] = databuff->getUint( stri1, j ) + temporaryIndices[j*3];
				temporaryIndices[j*3+2] = databuff->getUint( stri2, j ) + temporaryIndices[j*3];
			}

			databuff->deleteUintArray( stri0 );
			databuff->deleteUintArray( stri1 );
			databuff->deleteUintArray( stri2 );

			scene->meshes[i].vboTriangleIndices->load( vboIndex, scene->meshes[i].numTriangles*12, temporaryIndices );

			// read vertex data
			scene->meshes[i].vboVertexData = new meshVBO;
			scene->meshes[i].vboVertexData->load( vboVertex, scene->meshes[i].numVertex*(12+12+16+16), NULL );
			scene->meshes[i].vboVertexAttributes = new meshAttributeSet;

			unsigned int vertX = databuff->readQuantizedArray( infile );
			unsigned int vertY = databuff->readQuantizedArray( infile );
			unsigned int vertZ = databuff->readQuantizedArray( infile );

			scene->meshes[i].boundingBox.bmin[0] = databuff->getMin( vertX );
			scene->meshes[i].boundingBox.bmin[1] = databuff->getMin( vertY );
			scene->meshes[i].boundingBox.bmin[2] = databuff->getMin( vertZ );
			scene->meshes[i].boundingBox.bmax[0] = databuff->getMax( vertX );
			scene->meshes[i].boundingBox.bmax[1] = databuff->getMax( vertY );
			scene->meshes[i].boundingBox.bmax[2] = databuff->getMax( vertZ );

			/*
			printf( "mesh %d (%u vtx %u tri) [%3.2f : %3.2f] [%3.2f : %3.2f] [%3.2f : %3.2f]\n",
			i, scene->meshes[i].numVertex, scene->meshes[i].numTriangles,
			scene->meshes[i].boundingBox.bmin[0], scene->meshes[i].boundingBox.bmax[0],
			scene->meshes[i].boundingBox.bmin[1], scene->meshes[i].boundingBox.bmax[1],
			scene->meshes[i].boundingBox.bmin[2], scene->meshes[i].boundingBox.bmax[2] );
			*/

			vec3 *temporaryVertex = new vec3 [scene->meshes[i].numVertex];

			for (unsigned int j=0; j<scene->meshes[i].numVertex; j++)
			{
				temporaryVertex[j][0] = databuff->getFloat( vertX, j );
				temporaryVertex[j][1] = databuff->getFloat( vertY, j );
				temporaryVertex[j][2] = databuff->getFloat( vertZ, j );
			}

			databuff->deleteFloatArray( vertX );
			databuff->deleteFloatArray( vertY );
			databuff->deleteFloatArray( vertZ );

			scene->meshes[i].idxVertex = scene->meshes[i].vboVertexAttributes->addVertexAttribute( std::string("vertex"), vaFloat, 3, scene->meshes[i].numVertex );
			scene->meshes[i].vboVertexAttributes->updateVertexAttribute( scene->meshes[i].idxVertex, scene->meshes[i].vboVertexData, scene->meshes[i].numVertex*12, temporaryVertex );

			/// calculate normals.
			vec3 *temporaryNormals = new vec3 [scene->meshes[i].numVertex];

			for (unsigned int j=0; j<scene->meshes[i].numVertex; j++)
				temporaryNormals[j] = vec3( 0.0f, 0.0f, 0.0f );

			for (unsigned int j=0; j<scene->meshes[i].numTriangles; j++)
			{
				vec3 v1 = temporaryVertex[temporaryIndices[j*3+1]] - temporaryVertex[temporaryIndices[j*3]];
				vec3 v2 = temporaryVertex[temporaryIndices[j*3+2]] - temporaryVertex[temporaryIndices[j*3]];
				vec3 n = glm::normalize( glm::cross( v1, v2 ) );

				temporaryNormals[temporaryIndices[j*3]] += n;
				temporaryNormals[temporaryIndices[j*3+1]] += n;
				temporaryNormals[temporaryIndices[j*3+2]] += n;
			}

			for (unsigned int j=0; j<scene->meshes[i].numVertex; j++)
				temporaryNormals[j] = glm::normalize( temporaryNormals[j] );

			scene->meshes[i].idxNormals = scene->meshes[i].vboVertexAttributes->addVertexAttribute( std::string("normal"), vaFloat, 3, scene->meshes[i].numVertex );
			scene->meshes[i].vboVertexAttributes->updateVertexAttribute( scene->meshes[i].idxNormals, scene->meshes[i].vboVertexData, scene->meshes[i].numVertex*12, temporaryNormals );

			// read UV channels
			for (unsigned int j=0; j<4; j++)
			{
				infile->readUC( scene->meshes[i].numUVComponents[j] );
			}

			unsigned int *UVIDs = new unsigned int [3*4];

			for (unsigned int j=0; j<4; j++)
			{
				if (scene->meshes[i].numUVComponents[j] > 0)
					UVIDs[j*3] = databuff->readQuantizedArray( infile );
				if (scene->meshes[i].numUVComponents[j] > 1)
					UVIDs[j*3+1] = databuff->readQuantizedArray( infile );
				if (scene->meshes[i].numUVComponents[j] > 2)
					UVIDs[j*3+2] = databuff->readQuantizedArray( infile );
			}

			for (unsigned int j=0; j<4; j++)
			{
				const unsigned int numcomp = scene->meshes[i].numUVComponents[j];

				if (scene->meshes[i].numUVComponents[j] != 0)
				{
					float *uvw = new float [scene->meshes[i].numVertex*3];

					for (unsigned int k=0; k<scene->meshes[i].numVertex; k++)
					{
						uvw[k*3+0] = numcomp > 0 ? databuff->getFloat( UVIDs[j*3+0], k ) : 0.0f;
						uvw[k*3+1] = numcomp > 1 ? databuff->getFloat( UVIDs[j*3+1], k ) : 0.0f;
						uvw[k*3+2] = numcomp > 2 ? databuff->getFloat( UVIDs[j*3+2], k ) : 0.0f;
					}

					char vertexAttributeNameBuffer[16];
					sprintf( vertexAttributeNameBuffer, "uvw%u", j );

					scene->meshes[i].idxUvw[j] = scene->meshes[i].vboVertexAttributes->addVertexAttribute( std::string(vertexAttributeNameBuffer), vaFloat, 3, scene->meshes[i].numVertex );
					scene->meshes[i].vboVertexAttributes->updateVertexAttribute( scene->meshes[i].idxUvw[j], scene->meshes[i].vboVertexData, scene->meshes[i].numVertex*12, uvw );

					delete [] uvw;
				}
				else scene->meshes[i].idxUvw[j] = -1;
			}

			/// calculate tangent space. (tangent + w)
			if (scene->meshes[i].idxUvw[0] > 0)
			{
				vec3 *temporaryTan1 = new vec3 [scene->meshes[i].numVertex];
				vec3 *temporaryTan2 = new vec3 [scene->meshes[i].numVertex];

				for (unsigned int j=0; j<scene->meshes[i].numVertex; j++)
				{
					temporaryTan1[j] = vec3( 0.0f, 0.0f, 0.0f );
					temporaryTan2[j] = vec3( 0.0f, 0.0f, 0.0f );
				}

				for (unsigned int j=0; j<scene->meshes[i].numTriangles; j++)
				{
					unsigned int ind0 = temporaryIndices[j*3];
					unsigned int ind1 = temporaryIndices[j*3+1];
					unsigned int ind2 = temporaryIndices[j*3+2];

					vec3 v1 = temporaryVertex[ind0];
					vec3 v2 = temporaryVertex[ind1];
					vec3 v3 = temporaryVertex[ind2];

					vec2 w1 = vec2( databuff->getFloat( UVIDs[0], ind0 ), databuff->getFloat( UVIDs[1], ind0 ) );
					vec2 w2 = vec2( databuff->getFloat( UVIDs[0], ind1 ), databuff->getFloat( UVIDs[1], ind1 ) );
					vec2 w3 = vec2( databuff->getFloat( UVIDs[0], ind2 ), databuff->getFloat( UVIDs[1], ind2 ) );

					float x1 = v2[0] - v1[0];
					float x2 = v3[0] - v1[0];
					float y1 = v2[1] - v1[1];
					float y2 = v3[1] - v1[1];
					float z1 = v2[2] - v1[2];
					float z2 = v3[2] - v1[2];

					float s1 = w2[0] - w1[0];
					float s2 = w3[0] - w1[0];
					float t1 = w2[1] - w1[1];
					float t2 = w3[1] - w1[1];

					float r = 1.f / (s1 * t2 - s2 * t1);

					vec3 sdir( (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r );
					vec3 tdir( (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r );

					temporaryTan1[ind0] += sdir;
					temporaryTan1[ind1] += sdir;
					temporaryTan1[ind2] += sdir;

					temporaryTan2[ind0] += tdir;
					temporaryTan2[ind1] += tdir;
					temporaryTan2[ind2] += tdir;
				}

				vec4 *temporaryTangentW = new vec4 [scene->meshes[i].numVertex];

				for (unsigned int j=0; j<scene->meshes[i].numVertex; j++)
				{
					vec3 n = temporaryNormals[j];
					vec3 t = temporaryTan1[j];

					temporaryTangentW[j] = vec4( glm::normalize( t - n * glm::dot(n, t)), 
						glm::sign( glm::dot(glm::cross(n, t), temporaryTan2[j]) ) );
				}

				delete [] temporaryTan2;
				delete [] temporaryTan1;

				scene->meshes[i].idxTangentSpace = scene->meshes[i].vboVertexAttributes->addVertexAttribute( std::string("tangentw"), vaFloat, 4, scene->meshes[i].numVertex );
				scene->meshes[i].vboVertexAttributes->updateVertexAttribute( scene->meshes[i].idxTangentSpace, scene->meshes[i].vboVertexData, scene->meshes[i].numVertex*16, temporaryTangentW );

				delete [] temporaryTangentW;
			}

			/// cleanup
			for (unsigned int j=0; j<4; j++)
			{
				if (scene->meshes[i].numUVComponents[j] > 0)
					databuff->deleteFloatArray( UVIDs[j*3] );
				if (scene->meshes[i].numUVComponents[j] > 1)
					databuff->deleteFloatArray( UVIDs[j*3+1] );
				if (scene->meshes[i].numUVComponents[j] > 2)
					databuff->deleteFloatArray( UVIDs[j*3+2] );
			}

			delete [] UVIDs;
			delete [] temporaryNormals;
			delete [] temporaryIndices;
			delete [] temporaryVertex;

			/// read bones (for completeness at the moment)
			if (scene->meshes[i].numBones > 0)
			{
				unsigned int boneOffsetMatrix = databuff->readQuantizedArray( infile );

				for (unsigned int j=0; j<scene->meshes[i].numBones; j++)
				{
					unsigned int boneVertexIndices = databuff->readUintArray( infile );
					unsigned int boneVertexWeights = databuff->readQuantizedArray( infile );

					databuff->deleteUintArray( boneVertexIndices );
					databuff->deleteFloatArray( boneVertexWeights );
				}

				databuff->deleteFloatArray( boneOffsetMatrix );
			}
		}
	}

	static void importMaterials( iobFile *infile, DataBuffer* databuff, nsfScene *scene )
	{
		unsigned short nMaterials;
		infile->readUS( nMaterials );
		scene->numMaterials = nMaterials;
		scene->materials = new nsfMaterial [nMaterials];

		srand( 42 );
		printf( " ~ number of materials: %u\n", nMaterials );

		for (unsigned int i=0; i<scene->numMaterials; i++)
		{
			unsigned short nProperties;

			infile->readUS( nProperties );

			printf( " ~ -> material[%2d]: number of material properties: %u\n", i, nProperties );

			for (unsigned int j=0; j<nProperties; j++)
			{
				nsfMaterialProperty matprop;

				infile->readUI( matprop.type );
				infile->readStr( &matprop.name );
				infile->readUI( matprop.semantic );
				infile->readUI( matprop.index );
				infile->readUI( matprop.dataLength );

				printf( " ~ -> material[%2d] typ: %u sem: %u idx: %u name: %s\n", i, matprop.type, matprop.semantic, matprop.index, matprop.name );
				assert( matprop.dataLength <= 256 );

				matprop.data = (char*) malloc( matprop.dataLength+1 );
				matprop.data[matprop.dataLength] = 0;

				for (unsigned int k=0; k<matprop.dataLength; k++)
					infile->readC( matprop.data[k] );

				scene->materials[i].properties.push_back( matprop );
			}

			scene->materials[i].randomMaterialColor = rand() * rand();
		}
	}

	SCore::nsfScene* meshNSFLoader( const char* filename )
	{
		nsfScene* scene = NULL;
		SCore::DataBuffer* databuff = new SCore::DataBuffer;
		iobFile *infile = new SCore::iobFile( filename, true );

		char tag[4];

		infile->readC( tag[0] );
		infile->readC( tag[1] );
		infile->readC( tag[2] );
		infile->readC( tag[3] );

		if (tag[0] != 'N' || tag[1] != 'S' || tag[2] != 'F' || tag[3] != '2') return scene;

		scene = new nsfScene;

		importMeshes( infile, databuff, scene );
		importMaterials( infile, databuff, scene );

		delete infile;
		delete databuff;

		return scene;
	}

	/// Rendering code.
	void nsfScene::render( shader *s, int nLayers )
	{
		for (unsigned int i=0; i<numMeshes; i++)
			renderMesh( s, int(i), nLayers );
	}

	void nsfScene::renderMesh( shader *s, int meshNum, int nLayers )
	{
		if (meshNum >= 0 && meshNum < int(numMeshes))
		{
			const int i = meshNum;

			unsigned int randCol = materials[meshes[i].materialIdx].randomMaterialColor;
			glColor3ub( ((randCol>>16)&0x7F) + 128, ((randCol>>8)&0x7F) + 128, (randCol&0x7F) + 128 );

			meshes[i].vboTriangleIndices->bind();
			meshes[i].vboVertexData->bind();
			meshes[i].vboVertexAttributes->setAttributes( s );
			//glDrawElementsInstanced( GL_TRIANGLES, meshes[i].numTriangles*3, GL_UNSIGNED_INT, NULL, nLayers );
			glDrawElements( GL_TRIANGLES, meshes[i].numTriangles*3, GL_UNSIGNED_INT, NULL );
			meshes[i].vboVertexAttributes->unsetAttributes( s );
			meshes[i].vboVertexData->unbind();
			meshes[i].vboTriangleIndices->unbind();
		}
	}

};


