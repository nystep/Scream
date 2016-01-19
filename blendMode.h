/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   blendMode.h
 * Author: julien
 *
 * Created on January 16, 2016, 12:07 PM
 */

#ifndef BLENDMODE_H
#define BLENDMODE_H

namespace SCore {
    
    typedef enum {
        BLEND_REPLACE = 0,
        BLEND_ADD,
        BLEND_MUL,
        BLEND_XOR,
        BLEND_SCREEN,
        BLEND_OVERLAY,
        BLEND_DARKEN,
        BLEND_LIGHTEN,
        BLEND_COLORDODGE,
        BLEND_COLORBURN,
        BLEND_HARDLIGHT,
        BLEND_SOFTLIGHT,
        BLEND_DIFFERENCE,
        BLEND_EXCLUSION,
        BLEND_INVERT,
        BLEND_LINEARDODGE,
        BLEND_LINEARBURN,
        BLEND_VIVIDLIGHT,
        BLEND_LINEARLIGHT,
        BLEND_PINLIGHT,
        BLEND_HARDMIX,
    } BlendingMode;
    
    class blendMode {
    protected:
        //BlendingMode bm;
        unsigned int bm;

        unsigned int blendModeToGL( BlendingMode bm );
    public:
        
        blendMode( BlendingMode _bm ) : bm(blendModeToGL(_bm)) {
        }

        virtual ~blendMode() {
        }
        
        void set();
        void unset();

    };
    
}

#endif /* BLENDMODE_H */

