/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   layerParam.h
 * Author: julien
 *
 * Created on January 16, 2016, 9:13 AM
 */

#ifndef LAYERPARAM_H
#define LAYERPARAM_H

#include "blendMode.h"
#include "smath.h"

namespace SCore {

    class layerParam {
    protected:
        vec2 position;
        vec2 scale;
        vec3 hsb_bias;
        vec3 hsb_mod;
        vec4 color_bias;
        vec4 color_mod;
        blendMode blend;
    public:
        layerParam();
        layerParam(const layerParam& c);
        layerParam& operator= (const layerParam& c);
        ~layerParam();
    };

}

#endif /* LAYERPARAM_H */

