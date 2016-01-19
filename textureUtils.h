#pragma once


#include <string>


namespace SCore {

    class textureUtils
    {
    protected:
        bool hasWpExtension( const char *filename );
        std::string getWpFilename( const char *filename );
        bool convertImageToWp( const char *filename, bool *hasAlphaChannel, const float quality );
        bool upload2dImageWp( unsigned int target, const char *filename );
    };

}

