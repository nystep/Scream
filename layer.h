/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   layer.h
 * Author: julien
 *
 * Created on January 13, 2016, 6:31 PM
 */

#ifndef LAYER_H
#define LAYER_H

#include "layerResource.h"
#include "layerError.h"
#include <vector>

namespace SCore 
{

    class layer
    {
    protected:

        std::vector<layerResource*> inputs;
        std::vector<layerResource*> outputs;

        layerResource* getOutput( std::string name );
        layerResource* getInput( std::string name );

        std::vector<layerResource> default_inputs;
        std::vector<layerResource> default_outputs;

    public:

        layer();
        virtual ~layer();

        // let the layer provide default values for information/gui/fallback purposes
        virtual void initDefaults() = 0;
        
        virtual void initResources( std::string initStr ) = 0;
        virtual layerError errorId() = 0;
        virtual void cleanup() = 0;

        // for use for instancing layer resources by the director
        void addInput( layerResource *res );
        void addOutput( layerResource *res );
        
        // query default inputs
        virtual void enumerateInputsBegin( int &nInputs );
        virtual void enumerateInput( int inputId, layerResource &input );
        
        // query default outputs
        virtual void enumerateOutputsBegin( int &nOutputs );
        virtual void enumerateOutput( int outputId, layerResource &output );

    };

}

#include "layers.h"

#endif /* LAYER_H */

