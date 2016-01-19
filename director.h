/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   director.h
 * Description: code for the animation director
 * Author: julien perrier-cornet
 *
 * Created on January 11, 2016, 11:50 AM
 */

#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "layer.h"

namespace SCore 
{

    class director
    {
    protected:
        
        std::vector<layerResource*> resources;
        std::vector<layer*> layers;
        
    public:
        
        director();
        virtual ~director();
        
        void loadFromXml( const std::string& filename );
        void updateAnimations( double time );
        layer* getTopLayer();

    };

}


#endif /* DIRECTOR_H */

