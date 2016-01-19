
#include "screamCore.h"
#include "meshDistanceTransform.h"

namespace SCore
{

    shader *meshDistanceTransform::shDualDepth=0;
    shaderTextureSet *meshDistanceTransform::texDualCSM=0;
    shaderUniformSet *meshDistanceTransform::uniDualCSM=0;
    shader *meshDistanceTransform::shDualCSM=0;

    int meshDistanceTransform::texTemporaryRes=0;
    texture2d *meshDistanceTransform::texTemporaryDualHM=0;
    texture2d *meshDistanceTransform::texTemporaryDualCSM=0;

    int meshDistanceTransform::load( int resolution, nsfScene *m, int meshNumber )
    {
        /// create shaders
        if (!shDualCSM)
        {
            std::vector<shaderInfo> shadersnfo;
            
            shadersnfo.push_back( shaderInfo("shCsmDual.vert",GL_VERTEX_SHADER) );
            shadersnfo.push_back( shaderInfo("shCsmDual.frag",GL_FRAGMENT_SHADER) );

            shDualCSM = new SCore::shader;
            int result = shDualCSM->load( shadersnfo );

            if (result != SHADER_NO_ERRORS)
                return -1;
        }

        if (!shDualDepth)
        {
            std::vector<shaderInfo> shadersnfo;
            
            shadersnfo.push_back( shaderInfo("shDualDepth.vert",GL_VERTEX_SHADER) );
            shadersnfo.push_back( shaderInfo("shDualDepth.frag",GL_FRAGMENT_SHADER) );

            shDualDepth = new SCore::shader;
            int result = shDualDepth->load( shadersnfo );

            if (result != SHADER_NO_ERRORS)
                return -1;
        }

        /// benchmarking: timing query.
        unsigned int glTimingQuery;
        GLuint64 timeElapsed;
        glGenQueries( 1, &glTimingQuery );
        glBeginQuery( GL_TIME_ELAPSED, glTimingQuery );

        /// determine voxel resolution, bounding box.
        resolution = 1 << getPOTHigher( resolution );
        if (resolution < 16) return -1;

        /// sanity checks
        if (m == NULL) return -1;
        if (meshNumber < 0 || meshNumber >= int(m->numMeshes)) return -1;

        meshBoundingBox = m->meshes[meshNumber].boundingBox;
        meshBoundingBox.scale( float(resolution)/(resolution-5) );
        //meshBoundingBox.scaleForRotation();

        /// allocate or reallocate temporary textures
        if (resolution != texTemporaryRes)
        {
            if (texTemporaryDualHM) delete texTemporaryDualHM;
            if (texTemporaryDualCSM) delete texTemporaryDualCSM;
            if (texDualCSM) delete texDualCSM;

            texTemporaryDualHM = new texture2d( resolution, resolution, TEX_RG_F32 );

            texDualCSM = new shaderTextureSet;
            texDualCSM->add( texTemporaryDualHM, "heightmap", 0 );

            texTemporaryDualCSM = new texture2d( resolution, resolution, TEX_RGBA_F32 );

            texTemporaryRes = resolution;
        }

        /// uniforms
        if (!uniDualCSM)
        {
            uniDualCSM = new shaderUniformSet;
            uniDualCSM->add( UNIFORM_VEC3, "bboxmin", &meshBoundingBox.bmin );
            uniDualCSM->add( UNIFORM_VEC3, "bboxmax", &meshBoundingBox.bmax );
        }

        /// render to texture: dual depth
        unsigned int fboDualDepth;

        glGenFramebuffers( 1, &fboDualDepth );
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fboDualDepth );
        glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texTemporaryDualHM->textureId, 0 );

		glPushAttrib( GL_VIEWPORT_BIT );
		glViewport( 0, 0, resolution, resolution );
        glClear( GL_COLOR_BUFFER_BIT );

        glBlendEquation( GL_MAX );
        glEnable( GL_BLEND );

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();
        glOrtho( meshBoundingBox.bmin[0], meshBoundingBox.bmax[0],
                 meshBoundingBox.bmin[1], meshBoundingBox.bmax[1],
                 meshBoundingBox.bmin[2], meshBoundingBox.bmax[2] );
        glUseProgram( shDualDepth->programID );
        m->render( shDualDepth, 1 );
        glUseProgram( 0 );
        glPopMatrix();

        glDisable( GL_BLEND );

        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glPopAttrib();

        glDeleteFramebuffers( 1, &fboDualDepth );
        assert( glGetError() == GL_NO_ERROR );

        /// generate maximum mipmaps
        texTemporaryDualHM->generateMipmapsMax();
        //texTemporaryDualHM->generateMipmaps();

        assert( glGetError() == GL_NO_ERROR );

        /// compute a dual cone step map
        unsigned int fboDualCSM;

        glGenFramebuffers( 1, &fboDualCSM );
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fboDualCSM );
        glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texTemporaryDualCSM->textureId, 0 );

		glPushAttrib( GL_VIEWPORT_BIT );
		glViewport( 0, 0, resolution, resolution );
		glClear( GL_COLOR_BUFFER_BIT );

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();

        static float quadTexCoords[8] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
        static float quadVertex[12] = { -1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f };
        static unsigned int quadIndices[6] = { 0, 1, 2, 0, 2, 3 };

        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 3, GL_FLOAT, 0, quadVertex );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer( 2, GL_FLOAT, 0, quadTexCoords );

        glUseProgram( shDualCSM->programID );
        uniDualCSM->setUniforms( shDualCSM );
        texDualCSM->setTextures( shDualCSM );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, quadIndices );
        texDualCSM->unSetTextures();
        glUseProgram( 0 );

        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glDisableClientState( GL_VERTEX_ARRAY );

        glPopMatrix();
		glPopAttrib();

        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
        glDeleteFramebuffers( 1, &fboDualCSM );

        /// timing query
        glEndQuery( GL_TIME_ELAPSED );

        int available=0;

        while (!available) {
            glGetQueryObjectiv( glTimingQuery, GL_QUERY_RESULT_AVAILABLE, &available);
        }

        glGetQueryObjectui64v(glTimingQuery, GL_QUERY_RESULT, &timeElapsed );

        printf( "generated a %ux%u mesh distance map in %u microseconds...\n", resolution, resolution, unsigned(timeElapsed/1000L) );

        return glGetError() == GL_NO_ERROR;
    }

};

