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

using namespace std;
//setting up constants
const GLuint WIDTH = 1280, HEIGHT = 1280;
const GLuint RECNUM = 10000;
const GLuint RECSIZE = 25;
//declare functions and const parameters.
void key_exit(GLFWwindow* window, int key, int scancode,int action,int mode);
void key_start(GLFWwindow* window, int key, int scancode, int action, int mode);
void render(GLuint shaderProgram,GLuint VAO, vector<glm::vec4> colors);
vector<glm::vec4> generateColor();

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
