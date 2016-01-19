/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "layer.h"

namespace SCore 
{
    layer::layer() : inputs(), outputs(), default_inputs(), default_outputs() {
        
    }

    layer::~layer() {
    }

    void layer::addInput( layerResource *res ) {
        inputs.push_back(res);
    }

    void layer::addOutput( layerResource *res ) {
        outputs.push_back(res);
    }

    void layer::enumerateInputsBegin( int &nInputs ) {
        nInputs = static_cast<int>( default_inputs.size() );
    }
    
    void layer::enumerateInput( int inputId, layerResource &input ) {
        if (inputId >= 0 && inputId < static_cast<int>( default_inputs.size() )) {
            input = default_inputs[inputId];
        }
    }

    void layer::enumerateOutputsBegin( int &nOutputs ) {
        nOutputs = static_cast<int>( default_outputs.size() );
    }

    void layer::enumerateOutput( int outputId, layerResource &output ) {
        if (outputId >= 0 && outputId < static_cast<int>( default_outputs.size() )) {
            output = default_outputs[outputId];
        }
    }

    // query the output
    layerResource* getOutput( std::string name ) {
        
    }

    // query the input according to the layer rendering graph
    layerResource* getInput( std::string name ) {
        
    }

}
