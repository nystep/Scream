#pragma once

namespace SCore {

    class meshGpu
    {
        meshVBO vertexData;
        meshVBO indexData;

        meshAttributeSet *memoryAttributeLayout;

    public:

        meshGpu() : vertexData(), indexData() {}
        ~meshGpu() {}

        /// TODO: loading..

        void render( unsigned int nInstances );

    };

};
