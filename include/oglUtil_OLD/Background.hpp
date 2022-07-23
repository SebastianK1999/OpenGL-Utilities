#ifndef BACKGROUND_INCLUDED
#define BACKGROUND_INCLUDED

#include "oglUtil/drawables/DrawableBase.hpp"

#include "oglUtil_OLD/Misc.hpp"

#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

class Background : public oglu::DrawableBase {
    GLdouble xs[3];
    GLdouble ys[3];
    GLdouble alphas[3];

    void move();
   void setShaders();

public:
   Background();
   void draw(glm::mat4& MVP);

};
#endif