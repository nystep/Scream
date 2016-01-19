/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "screamCore.h"
#include "layer.h"

namespace SCore {

    // float
    template <> const float& layerResourceQuery<float>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<float>::set( const float& v )
    {
        value = v;
    }

    // int
    template <> const int& layerResourceQuery<int>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<int>::set( const int& v )
    {
        value = v;
    }

    // vec2
    template <> const vec2& layerResourceQuery<vec2>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<vec2>::set( const vec2& v )
    {
        value = v;
    }
    
    // vec3
    template <> const vec3& layerResourceQuery<vec3>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<vec3>::set( const vec3& v )
    {
        value = v;
    }
    
    // vec4
    template <> const vec4& layerResourceQuery<vec4>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<vec4>::set( const vec4& v )
    {
        value = v;
    }
    
    // camera
    template <> const camera& layerResourceQuery<camera>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<camera>::set( const camera& v )
    {
        value = v;
    }
    
    // texture
    template <> const texture& layerResourceQuery<texture>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<texture>::set( const texture& v )
    {
        value = v;
    }
    
    // std::string
    template <> const std::string& layerResourceQuery<std::string>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<std::string>::set( const std::string& v )
    {
        value = v;
    }

    // canvas
    template <> const canvas& layerResourceQuery<canvas>::get()
    {
        return value;
    }

    template <> void layerResourceQuery<canvas>::set( const canvas& v )
    {
        value = v;
    }

    // 
    layerResourceInt::layerResourceInt( int64_t _instanceId, std::string _name, int _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceInt::~layerResourceInt() {
    }

    // 
    layerResourceFloat::layerResourceFloat( int64_t _instanceId, std::string _name, float _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceFloat::~layerResourceFloat() {
    }

    // 
    layerResourceVec2::layerResourceVec2( int64_t _instanceId, std::string _name, vec2 _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceVec2::~layerResourceVec2() {
    }

    layerResourceVec3::layerResourceVec3( int64_t _instanceId, std::string _name, vec3 _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceVec3::~layerResourceVec3() {
    }

    layerResourceVec4::layerResourceVec4( int64_t _instanceId, std::string _name, vec4 _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceVec4::~layerResourceVec4() {
    }

    layerResourceCamera::layerResourceCamera( int64_t _instanceId, std::string _name, camera _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceCamera::~layerResourceCamera() {
    }

    layerResourceTexture::layerResourceTexture( int64_t _instanceId, std::string _name, texture _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceTexture::~layerResourceTexture() {
    }

    layerResourceString::layerResourceString( int64_t _instanceId, std::string _name, std::string _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceString::~layerResourceString() {
    }

    layerResourceCanvas::layerResourceCanvas( int64_t _instanceId, std::string _name, canvas _value ) : layerResource(_instanceId, _name) {
        set(_value);
    }

    layerResourceCanvas::~layerResourceCanvas() {
    }

}
