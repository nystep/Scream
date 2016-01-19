
#include "screamCore.h"
#include "meshDDPTransform.h"

namespace SCore {

    shader *meshDDPTransform::shDualDepth=0;
    shaderTextureSet *meshDDPTransform::texDualDepthPeel=0;
    shader *meshDDPTransform::shClearTextureSet=0;
    shaderTextureSet *meshDDPTransform::texClearTextureSet=0;

    int meshDDPTransform::texTemporaryRes=0;
    texture2d *meshDDPTransform::texTemporaryDualHMP=0;
    texture2d *meshDDPTransform::texTemporaryDualHMN=0;
    texture2d *meshDDPTransform::texTemporaryDualHMP_u=0;
    texture2d *meshDDPTransform::texTemporaryDualHMN_u=0;
    texture2d *meshDDPTransform::texTemporaryDualHMP_v=0;
    texture2d *meshDDPTransform::texTemporaryDualHMN_v=0;


    void meshDDPTransform::generateMaximumMipmaps( texture2d *tex1, texture2d *tex2 )
    {
        static shader* shMaxMipmap2;
        static int imageUpper1Loc;
        static int imageUpper2Loc;
        static int imageLower1Loc;
        static int imageLower2Loc;

        if (!shMaxMipmap2)
        {
            std::vector<shaderInfo> shadersnfo;
            
            shadersnfo.push_back( shaderInfo("shMaxMipmap2.vert",GL_VERTEX_SHADER) );
            shadersnfo.push_back( shaderInfo("shMaxMipmap2.frag",GL_FRAGMENT_SHADER) );
            
            shMaxMipmap2 = new shader;
            shMaxMipmap2->load( shadersnfo );

            imageUpper1Loc = glGetUniformLocation( shMaxMipmap2->programID, "imageUpper1" );
            imageUpper2Loc = glGetUniformLocation( shMaxMipmap2->programID, "imageUpper2" );
            imageLower1Loc = glGetUniformLocation( shMaxMipmap2->programID, "imageLower1" );
            imageLower2Loc = glGetUniformLocation( shMaxMipmap2->programID, "imageLower2" );
        }

        int renderSx, renderSy;

        glBindTexture( tex1->target, tex1->textureId );
        glGetTexLevelParameteriv( tex1->target, 0, GL_TEXTURE_WIDTH, &renderSx );
        glGetTexLevelParameteriv( tex1->target, 0, GL_TEXTURE_HEIGHT, &renderSy );
        glBindTexture( tex1->target, 0 );

        int maxMipmapLevel = int( glm::max( glm::log2( float(renderSx) ), glm::log2( float(renderSy) ) ) );

        glPushAttrib( GL_VIEWPORT_BIT );

        for (int i=1; i<=maxMipmapLevel; i++)
        {
            int im1 = i-1;
            renderSx /= 2;
            renderSy /= 2;

            /// Render to the current mipmap level
            glViewport( 0, 0, renderSx, renderSy );

            /// Set the shader
            glUseProgram( shMaxMipmap2->programID );

            glBindImageTexture( 0, tex1->textureId, i, GL_FALSE, 0, GL_WRITE_ONLY, tex1->internalFormat );
            glUniform1i( imageLower1Loc, 0 );
            glBindImageTexture( 1, tex2->textureId, i, GL_FALSE, 0, GL_WRITE_ONLY, tex2->internalFormat );
            glUniform1i( imageLower2Loc, 1 );
            glBindImageTexture( 2, tex1->textureId, im1, GL_FALSE, 0, GL_READ_ONLY, tex1->internalFormat );
            glUniform1i( imageUpper1Loc, 2 );
            glBindImageTexture( 3, tex2->textureId, im1, GL_FALSE, 0, GL_READ_ONLY, tex2->internalFormat );
            glUniform1i( imageUpper2Loc, 3 );

            glRecti( -1, -1, 1, 1 );

            /// Memory access synchronization
            glMemoryBarrier( GL_TEXTURE_FETCH_BARRIER_BIT |
                             GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                             GL_PIXEL_BUFFER_BARRIER_BIT |
                             GL_TEXTURE_UPDATE_BARRIER_BIT |
                             GL_BUFFER_UPDATE_BARRIER_BIT |
                             GL_FRAMEBUFFER_BARRIER_BIT );

            glUseProgram( 0 );

            assert( glGetError() == GL_NO_ERROR );
        }

        glPopAttrib();

#if 0
        /// TESTING / VALIDATING
        glBindTexture( tex1->target, tex1->textureId );
        glGetTexLevelParameteriv( tex1->target, 0, GL_TEXTURE_WIDTH, &renderSx );
        glGetTexLevelParameteriv( tex1->target, 0, GL_TEXTURE_HEIGHT, &renderSy );

        float *buff = new float [renderSx*renderSy];

        for (int i=0; i<maxMipmapLevel; i++)
        {
            glGetTexImage( tex1->target, i, GL_RED, GL_FLOAT, buff );

            //int incrX = renderSx/8 < 1 ? 1 : renderSx/8;
            //int incrY = renderSy/8 < 1 ? 1 : renderSy/8;
            int limitX = renderSx<8 ? renderSx:8;
            int limitY = renderSy<8 ? renderSy:8;

            printf( " # buffer contents at level %d:\n", i );

            for (int y=0; y<limitY; y++)
            {
                for (int x=0; x<limitX; x++)
                {
                    int indice = y*renderSx + x;

                    printf( "%6f ", buff[indice] );
                }
                printf( "\n" );
            }

            renderSx /= 2;
            renderSy /= 2;
            if (renderSx == 0 || renderSy == 0) break;
        }

        delete [] buff;
        glBindTexture( tex1->target, 0 );
#endif
    }


    int meshDDPTransform::load( int resolution, nsfScene *m, int meshNumber )
    {
        /// initialize the shader.
        if (!shDualDepth)
        {
            std::vector<shaderInfo> shadersnfo;
            
            shadersnfo.push_back( shaderInfo("shDualDepthT.vert",GL_VERTEX_SHADER) );
            shadersnfo.push_back( shaderInfo("shDualDepthT.frag",GL_FRAGMENT_SHADER) );

            shDualDepth = new SCore::shader;
            if (shDualDepth->load( shadersnfo ) != SHADER_NO_ERRORS) return 0;
        }

        if (!shClearTextureSet)
        {
            std::vector<shaderInfo> shadersnfo;
            
            shadersnfo.push_back( shaderInfo("shClearTexturesDDT.vert",GL_VERTEX_SHADER) );
            shadersnfo.push_back( shaderInfo("shClearTexturesDDT.frag",GL_FRAGMENT_SHADER) );

            shClearTextureSet = new SCore::shader;
            if (shClearTextureSet->load( shadersnfo ) != SHADER_NO_ERRORS) return 0;
        }

        /// benchmarking: timing query.
        unsigned int glTimingQuery;
        GLuint64 timeElapsed;
        glGenQueries( 1, &glTimingQuery );
        glBeginQuery( GL_TIME_ELAPSED, glTimingQuery );

        /// determine voxel resolution, bounding box.
        resolution = 1 << getPOTHigher( resolution );
        if (resolution < 16) return 0;

        /// sanity checks
        if (m == NULL) return 0;
        if (meshNumber < 0 || meshNumber > int(m->numMeshes)) return 0;

        meshBoundingBox = m->meshes[0].boundingBox;
        meshBoundingBox.scale( float(resolution)/(resolution-5) );
        //meshBoundingBox.scaleForRotation();

        /// allocate or reallocate temporary textures
        if (resolution != texTemporaryRes)
        {
            textureInternalFormat tif = TEX_RGBA_F32;
            textureFiltering sf = filterMipmaps;
            textureWrapMode wm = wrapClamp;

            if (texTemporaryDualHMP) delete texTemporaryDualHMP;
            texTemporaryDualHMP = new texture2d( resolution, resolution, tif, sf, wm );
            if (texTemporaryDualHMN) delete texTemporaryDualHMN;
            texTemporaryDualHMN = new texture2d( resolution, resolution, tif, sf, wm );
            if (texTemporaryDualHMP_u) delete texTemporaryDualHMP_u;
            texTemporaryDualHMP_u = new texture2d( resolution, resolution, tif, sf, wm );
            if (texTemporaryDualHMN_u) delete texTemporaryDualHMN_u;
            texTemporaryDualHMN_u = new texture2d( resolution, resolution, tif, sf, wm );
            if (texTemporaryDualHMP_v) delete texTemporaryDualHMP_v;
            texTemporaryDualHMP_v = new texture2d( resolution, resolution, tif, sf, wm );
            if (texTemporaryDualHMN_v) delete texTemporaryDualHMN_v;
            texTemporaryDualHMN_v = new texture2d( resolution, resolution, tif, sf, wm );

            texTemporaryRes = resolution;

            if (!texDualDepthPeel)
            {
                texDualDepthPeel = new SCore::shaderTextureSet;
                texDualDepthPeel->addLoadStore( texTemporaryDualHMP,
                                                std::string( "imageDepthPeelPos" ),
                                                lsReadWrite, 0 );
                texDualDepthPeel->addLoadStore( texTemporaryDualHMN,
                                                std::string( "imageDepthPeelNeg" ),
                                                lsReadWrite, 1 );
                texDualDepthPeel->addLoadStore( texTemporaryDualHMP_u,
                                                std::string( "imageDepthPeelPos_u" ),
                                                lsReadWrite, 2 );
                texDualDepthPeel->addLoadStore( texTemporaryDualHMN_u,
                                                std::string( "imageDepthPeelNeg_u" ),
                                                lsReadWrite, 3 );
                texDualDepthPeel->addLoadStore( texTemporaryDualHMP_v,
                                                std::string( "imageDepthPeelPos_v" ),
                                                lsReadWrite, 4 );
                texDualDepthPeel->addLoadStore( texTemporaryDualHMN_v,
                                                std::string( "imageDepthPeelNeg_v" ),
                                                lsReadWrite, 5 );
            }

            if (!texClearTextureSet)
            {
                texClearTextureSet = new SCore::shaderTextureSet;
                texClearTextureSet->addLoadStore( texTemporaryDualHMP,
                                                std::string( "imageDepthPeelPos" ),
                                                lsWrite, 0 );
                texClearTextureSet->addLoadStore( texTemporaryDualHMN,
                                                std::string( "imageDepthPeelNeg" ),
                                                lsWrite, 1 );
                texClearTextureSet->addLoadStore( texTemporaryDualHMP_u,
                                                std::string( "imageDepthPeelPos_u" ),
                                                lsWrite, 2 );
                texClearTextureSet->addLoadStore( texTemporaryDualHMN_u,
                                                std::string( "imageDepthPeelNeg_u" ),
                                                lsWrite, 3 );
                texClearTextureSet->addLoadStore( texTemporaryDualHMP_v,
                                                std::string( "imageDepthPeelPos_v" ),
                                                lsWrite, 4 );
                texClearTextureSet->addLoadStore( texTemporaryDualHMN_v,
                                                std::string( "imageDepthPeelNeg_v" ),
                                                lsWrite, 5 );
            }
        }

        /// dual depth peeling with shader_image_load_store
		glPushAttrib( GL_VIEWPORT_BIT );
		glViewport( 0, 0, resolution, resolution );

        /// First step: clear the texture
        texClearTextureSet->setTextures( shClearTextureSet );
        glRecti( -1, -1, 1, 1 );
        texClearTextureSet->unSetTextures();
        glUseProgram( 0 );

        /// Second step: render to main framebuffer
        glMatrixMode( GL_PROJECTION );
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();

        glOrtho( meshBoundingBox.bmin[0], meshBoundingBox.bmax[0],
                 meshBoundingBox.bmin[1], meshBoundingBox.bmax[1],
                 meshBoundingBox.bmin[2], meshBoundingBox.bmax[2] );

        glUseProgram( shDualDepth->programID );
        texDualDepthPeel->setTextures( shDualDepth );
        m->render( shDualDepth, meshNumber );
        texDualDepthPeel->unSetTextures();
        glUseProgram( 0 );

        glPopMatrix();
        glMatrixMode( GL_PROJECTION );
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );

		glPopAttrib(); //GL_VIEWPORT_BIT

        /// generate mipmaps
#if 0
        texTemporaryDualHMP->generateMipmaps();
        texTemporaryDualHMN->generateMipmaps();
        texTemporaryDualHMP_u->generateMipmaps();
        texTemporaryDualHMN_u->generateMipmaps();
        texTemporaryDualHMP_v->generateMipmaps();
        texTemporaryDualHMN_v->generateMipmaps();
#else
        generateMaximumMipmaps( texTemporaryDualHMP, texTemporaryDualHMN );
        texTemporaryDualHMP_u->generateMipmaps();
        texTemporaryDualHMN_u->generateMipmaps();
        texTemporaryDualHMP_v->generateMipmaps();
        texTemporaryDualHMN_v->generateMipmaps();
#endif

        /// timing query
        glEndQuery( GL_TIME_ELAPSED );

        int available=0;
        while (!available) {
            glGetQueryObjectiv( glTimingQuery, GL_QUERY_RESULT_AVAILABLE, &available);
        }
        glGetQueryObjectui64v(glTimingQuery, GL_QUERY_RESULT, &timeElapsed );

        printf( "generated a %ux%u dual depth peeling texture in %u microseconds...\n", resolution, resolution, unsigned(timeElapsed/1000L) );

        return glGetError() == GL_NO_ERROR;
    }

}
