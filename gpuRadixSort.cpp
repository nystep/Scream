
#include "texture.h"
#include "shader.h"
#include <assert.h>
#include <iostream>


namespace SCore {

    void gpuRadixSort( unsigned int *destinationIndices, unsigned int *sourceData, unsigned int sz )
    {
        int available = 0;
        unsigned int glTimingQuery[5];
        GLuint64 timeElapsed[5];

        glGenQueries( 5, glTimingQuery );

        if (!isPOT(sz))
        {
            std::cerr << "gpuRadixSort error: the size of the array to sort isn't a power of 2" << std::endl;
            return;
        }

        int szPot = getPOTHigher( sz );
        int szs2 = szPot >> 1;
        int width = 1 << szs2;
        int height = sz / width;

        //std::cout << "szPot " << szPot << " szs2 " << szs2 << " width " << width << " height " << height << std::endl;

        SCore::texture2d *srcData0 = new SCore::texture2d( width, height,
                                                           TEX_LUMI_UI32,
                                                           SCore::filterNearest,
                                                           SCore::wrapClamp,
                                                           (void*) sourceData );

        SCore::texture2d *radixStatsTexture = new SCore::texture2d( 4, 4, TEX_LUMI_UI32, SCore::filterNearest, SCore::wrapClamp, 0 );

        SCore::shaderTextureSet *textureSetMakeStats = new SCore::shaderTextureSet;
        textureSetMakeStats->addLoadStore( radixStatsTexture, "statistics", lsWrite, 0 );
        textureSetMakeStats->add( srcData0, "data", 1 );

        SCore::shader *radixStatsSh = new SCore::shader;
        radixStatsSh->load( shaderInfo( "radixStats.frag", GL_FRAGMENT_SHADER ) );

        delete radixStatsSh;
        delete textureSetMakeStats;
        delete radixStatsTexture;
        delete srcData0;

#if 0
        SCore::meshVBO *vboSource0 = new SCore::meshVBO;
        vboSource0->load( vboTextureBuffer, sz*sizeof(unsigned int), sourceData );

        std::cout << "printing out contents of sourceData" << std::endl;
        for (int i=0; i<sz; i++)
            std::cout << "sourceData[" << i << "] = " << sourceData[i] << std::endl;
		std::cout << std::endl;

        SCore::textureBuffer* srcData0 = new SCore::textureBuffer( vboSource0, TEX_LUMI_UI32 );

        unsigned int *content = new unsigned int [sz];
		glBindTexture( srcData0->target, srcData0->textureId );
		glGetTexImage( srcData0->target, 0, GL_LUMINANCE_INTEGER_EXT, GL_UNSIGNED_INT, content );
		glBindTexture( srcData0->target, 0 );
        std::cout << "printing out contents of srcData0" << std::endl;
		for (unsigned int i=0; i<sz; i++)
		{
		    std::cout << content[i] << std::endl;
		}
		std::cout << std::endl;
		delete [] content;

        SCore::meshVBO *vboSource1 = new SCore::meshVBO;
        vboSource1->load( vboTextureBuffer, sz*sizeof(unsigned int), NULL );
        SCore::textureBuffer* srcData1 = new SCore::textureBuffer( vboSource1, TEX_LUMI_UI32 );

        SCore::texture2d* radixStatsTexture = new SCore::texture2d( 256, 4, TEX_LUMI_UI32, SCore::filterNearest, SCore::wrapClamp, 0 );

        SCore::shaderTextureSet *textureSetOnlySrc = new SCore::shaderTextureSet;
        textureSetOnlySrc->addLoadStore( radixStatsTexture, "statistics", lsWrite, 0 );
        textureSetOnlySrc->addLoadStore( srcData0, "src", lsRead, 1 );

        SCore::shaderUniformSet *uniformSetSz = new SCore::shaderUniformSet;
        uniformSetSz->add( UNIFORM_INT, "sz", &sz );
        //uniformSetSz->addInt( "sz", sz );

        SCore::shader* radixStatsSh = new SCore::shader;
        radixStatsSh->load( "radixStats.frag" );

        /// calculate statistics on input stream.
        assert( glGetError() == GL_NO_ERROR );
        glBeginQuery( GL_TIME_ELAPSED, glTimingQuery[0] );
        radixStatsSh->computeShader( 32, 32, textureSetOnlySrc, uniformSetSz );
        glEndQuery( GL_TIME_ELAPSED );
        assert( glGetError() == GL_NO_ERROR );

        while (!available) {
            glGetQueryObjectiv(glTimingQuery[0], GL_QUERY_RESULT_AVAILABLE, &available);
        }

        glGetQueryObjectui64v(glTimingQuery[0], GL_QUERY_RESULT, &timeElapsed[0]);

        std::cout << "made statistics in " << int(timeElapsed[0]/1000L) << " microseconds" << std::endl;

        unsigned int *stats = new unsigned int [1024];
		glBindTexture( GL_TEXTURE_2D, radixStatsTexture->textureId );
		glGetTexImage( GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, stats );
		glBindTexture( GL_TEXTURE_2D, 0 );

        /*
		for (unsigned int i=0; i<1024; i++)
		{
		    if ((i&31) == 0) std::cout << std::endl;
		    std::cout << stats[i] << " ";
		}
		std::cout << std::endl;
		*/
		delete [] stats;
#else

#endif
    }

};
