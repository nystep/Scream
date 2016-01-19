/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "layer.h"

namespace SCore {

    layerImage::layerImage() {
        
    }

    layerImage::~layerImage() {
        
    }

    void layerImage::initDefaults() {
        std::vector<layerResource> default_inputs;
        std::vector<layerResource> default_outputs;
        
        layerResource lr;
        
        //default_inputs
    }

    void layerImage::initResources( std::string initStr ) {
        
    }

    layerError layerImage::errorId() {
        return LAYER_NOERROR;
    }

    void layerImage::cleanup() {
        
    }

}
