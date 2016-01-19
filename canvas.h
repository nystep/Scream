/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   canvas.h
 * Author: julien perrier-cornet
 *
 * Created on January 10, 2016, 10:59 AM
 */

#ifndef CANVAS_H
#define CANVAS_H

namespace SCore {

    class canvas {
    public:
        canvas();
        canvas(const canvas& c);
        canvas& operator= (const canvas& c);
        ~canvas();
    };

}

#endif /* CANVAS_H */

