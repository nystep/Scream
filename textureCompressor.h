/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   textureCompressor.h
 * Author: julien
 *
 * Created on January 11, 2016, 11:58 AM
 */

#ifndef TEXTURECOMPRESSOR_H
#define TEXTURECOMPRESSOR_H

namespace Score {
    
    class texture2d;

    class textureCompressor {
    protected:
    public:

        textureCompressor( const texture2d &input );
        virtual ~textureCompressor();
        
        unsigned int getCompressedDataSize();
        void getCompressedData( unsigned char* buffer, unsigned int *compressedSize );

    };

}

#endif /* TEXTURECOMPRESSOR_H */

