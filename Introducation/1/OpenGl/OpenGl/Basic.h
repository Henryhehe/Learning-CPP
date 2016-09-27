//
//  Basic.h
//  OpenGl
//
//  Created by Chuanheng He on 2016-09-12.
//  Copyright © 2016 Chuanheng He. All rights reserved.
//

#ifndef Basic_h
#define Basic_h
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <vector>

struct validTriangle {
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;
};

struct validRectangle {
    glm::vec3 centre;
    float height;
    float width;
    glm::vec3 firstPoint;
    glm::vec3 secondPoint;
    glm::vec3 thirdPoint;
    glm::vec3 fouthPoint;
};


#endif /* Basic_h */
