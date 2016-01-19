/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "blendMode.h"
#include <GL/glew.h>

namespace SCore {

    unsigned int blendMode::blendModeToGL( BlendingMode bm ) {
        switch (bm) {
            case BLEND_ADD:
                return GL_SRC_NV;
            case BLEND_MUL:
                return GL_MULTIPLY_NV;
            case BLEND_XOR:
                return GL_XOR;
            case BLEND_SCREEN:
                return GL_SCREEN_NV;
            case BLEND_OVERLAY:
                return GL_OVERLAY_NV;
            case BLEND_DARKEN:
                return GL_DARKEN_NV;
            case BLEND_LIGHTEN:
                return GL_LIGHTEN_NV;
            case BLEND_COLORDODGE:
                return GL_COLORDODGE_NV;
            case BLEND_COLORBURN:
                return GL_COLORBURN_NV;
            case BLEND_HARDLIGHT:
                return GL_HARDLIGHT_NV;
            case BLEND_SOFTLIGHT:
                return GL_SOFTLIGHT_NV;
            case BLEND_DIFFERENCE:
                return GL_DIFFERENCE_NV;
            case BLEND_EXCLUSION:
                return GL_EXCLUSION_NV;
            case BLEND_INVERT:
                return GL_INVERT;
            case BLEND_LINEARDODGE:
                return GL_LINEARDODGE_NV;
            case BLEND_LINEARBURN:
                return GL_LINEARBURN_NV;
            case BLEND_VIVIDLIGHT:
                return GL_VIVIDLIGHT_NV;
            case BLEND_LINEARLIGHT:
                return GL_LINEARLIGHT_NV;
            case BLEND_PINLIGHT:
                return GL_PINLIGHT_NV;
            case BLEND_HARDMIX:
                return GL_HARDMIX_NV;
            case BLEND_REPLACE:
            default:
                return GL_SRC_NV;
        }
    }

    void blendMode::set() {
        glEnable( GL_BLEND_ADVANCED_COHERENT_NV );
        glBlendEquation( bm );
    }
    
    void blendMode::unset() {
        glDisable( GL_BLEND_ADVANCED_COHERENT_NV );
    }

}
