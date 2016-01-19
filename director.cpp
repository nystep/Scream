/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <string>
#include <vector>
#include "director.h"
#include "tinyxml2.h"
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace tinyxml2;

namespace SCore {

    director::director() : resources(), layers()
    {
    }

    director::~director()
    {
    }

    void director::loadFromXml( const std::string& filename )
    {
	XMLDocument doc;
	doc.LoadFile( filename.c_str() );

        if (!doc.Error()) {
            // Load layer instances
            // Make layer instance resource connections
            // Determine if there are any circular dependencies
        }
    }

    void director::updateAnimations( double time )
    {

    }

    layer* director::getTopLayer()
    {
        layer* dummy = 0;
        if (layers.size() > 0) dummy = layers[0];
        return dummy;
    }

}
