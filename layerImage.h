/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   layerImage.h
 * Author: julien
 *
 * Created on January 16, 2016, 12:41 AM
 */

#ifndef LAYERIMAGE_H
#define LAYERIMAGE_H

namespace SCore {
    
    class layerImage : public layer {
    protected:
    public:
        layerImage();
        ~layerImage();

        void initDefaults();
        void initResources( std::string initStr );
        layerError errorId();
        void cleanup();
    };
    
}

#endif /* LAYERIMAGE_H */

