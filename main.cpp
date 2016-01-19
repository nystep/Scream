/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: julien
 *
 * Created on January 12, 2016, 5:20 PM
 */

#include "screamCore.h"
using namespace SCore;

/*
 * 
 */
int main(int argc, char** argv) {

    contextConfig cfg;
    
    cfg.sx = 1920;
    cfg.sy = 1080;
    cfg.windowTitle = "screamCore test";

    context *ctxt = 0;
#ifdef _WINDOWS
    ctxt = new context_win32();
#else
    ctxt = new context_linux();
    
#endif
    
    if (ctxt->create( &cfg ))
    {
        while (ctxt->update()) ;
    }
    else
    {
        printf( "initialisation error\n" );
        return -1;
    }

    ctxt->destroy();

    return 0;
}

