#pragma once


namespace SCore {

    class meshViewpoint
    {
        vec3 vector;

        public:

            meshViewpoint( vec3 v );
            void rotate( vec3 axis, float angle );

    };

};
