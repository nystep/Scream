/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "layerParam.h"

namespace SCore {

    layerParam::layerParam() : 
        position(vec2(0.f)), scale(vec2(1.f)), hsb_bias(vec3(0.f)),
        hsb_mod(vec3(1.f)), color_bias(vec4(0.f)), color_mod(vec4(1.f)), 
        blend(BLEND_REPLACE)
    {
    }

    layerParam::layerParam(const layerParam& c) {
        position = c.position;
        scale = c.scale;
        hsb_bias = c.hsb_bias;
        hsb_mod = c.hsb_mod;
        color_bias = c.color_bias;
        color_mod = c.color_mod;
        blend = c.blend;
    }

    layerParam& layerParam::operator= (const layerParam& c) {
        position = c.position;
        scale = c.scale;
        hsb_bias = c.hsb_bias;
        hsb_mod = c.hsb_mod;
        color_bias = c.color_bias;
        color_mod = c.color_mod;
        blend = c.blend;
    }
    
    layerParam::~layerParam() {
        
    }
    
}
