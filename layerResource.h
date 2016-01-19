/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   layerResource.h
 * Author: julien
 *
 * Created on January 14, 2016, 10:24 AM
 */

#ifndef LAYERRESOURCE_H
#define LAYERRESOURCE_H

#include "stype.h"
#include "smath.h"
#include "camera.h"
#include "canvas.h"
#include "texture.h"
#include "layerParam.h"

namespace SCore {

    class layerResource {
    protected:
        int64_t instanceId, sourceLayerId;
        std::string name;
    public:
        layerResource( int64_t _instanceId, std::string _name ) : instanceId(_instanceId), name(_name) {
        }
        
        virtual ~layerResource() {
        }
    };
    
    template<typename T> class layerResourceQuery {
    protected:
        T value;
    public:
        layerResourceQuery() : value() {
        }

        ~layerResourceQuery() {
        }
        
        const T& get();
        void set( const T& v );
    };
    
    class layerResourceInt : public layerResource, public layerResourceQuery<int> {
    public:
        layerResourceInt( int64_t _instanceId, std::string _name, int _value=0 );
        ~layerResourceInt();
    };

    class layerResourceFloat : public layerResource, public layerResourceQuery<float> {
    public:
        layerResourceFloat( int64_t _instanceId, std::string _name, float _value=0.0 );
        ~layerResourceFloat();
    };
    
    class layerResourceVec2 : public layerResource, public layerResourceQuery<vec2> {
        layerResourceVec2( int64_t _instanceId, std::string _name, vec2 _value=vec2(0.0) );        
        ~layerResourceVec2();
    };

    class layerResourceVec3 : public layerResource, public layerResourceQuery<vec3> {
        layerResourceVec3( int64_t _instanceId, std::string _name, vec3 _value=vec3(0.0) );
        ~layerResourceVec3();
    };

    class layerResourceVec4 : public layerResource, public layerResourceQuery<vec4> {
        layerResourceVec4( int64_t _instanceId, std::string _name, vec4 _value=vec4(0.0) );
        ~layerResourceVec4();
    };

    class layerResourceCamera : public layerResource, public layerResourceQuery<camera> {
        layerResourceCamera( int64_t _instanceId, std::string _name, camera _value=camera() );
        ~layerResourceCamera();
    };

    class layerResourceTexture : public layerResource, public layerResourceQuery<texture> {
        layerResourceTexture( int64_t _instanceId, std::string _name, texture _value=texture() );
        ~layerResourceTexture();
    };

    class layerResourceString : public layerResource, public layerResourceQuery<std::string> {
        layerResourceString( int64_t _instanceId, std::string _name, std::string _value=std::string() );
        ~layerResourceString();
    };
    
    class layerResourceCanvas : public layerResource, public layerResourceQuery<canvas> {
        layerResourceCanvas( int64_t _instanceId, std::string _name, canvas _value=canvas() );
        ~layerResourceCanvas();
    };

}

#endif /* LAYERRESOURCE_H */

