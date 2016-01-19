/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   layerError.h
 * Author: julien perrier-cornet
 *
 * Created on January 15, 2016, 7:52 PM
 */

#ifndef LAYERERROR_H
#define LAYERERROR_H

namespace SCore {

    typedef enum {
        LAYER_NOERROR = 0,
        LAYER_NOTENOUGHMEMORY,
        LAYER_SHADERCOMPILATIONFAILURE,
        LAYER_FILENOTFOUND,
        //LAYER_
    } layerError;

}

#endif /* LAYERERROR_H */

