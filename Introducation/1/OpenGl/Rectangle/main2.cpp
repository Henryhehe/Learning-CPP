//
//  main.cpp
//  Rectangle
//
//  Created by Chuanheng He on 2016-09-23.
//  Student Number:250825763
//  Email Address: che49@uwo.ca
//  Copyright © 2016 Chuanheng He. All rights reserved.

#include "Basic.h"

using namespace std;
//declare functions and const parameters.
//void key_start(GLFWwindow* window, int key, int scancode, int action, int mode);

void key_exit(GLFWwindow* window, int key, int scancode,int action,int mode);
void key_start(GLFWwindow* window, int key, int scancode, int action, int mode);
void render(GLuint shaderProgram,GLuint VAO, vector<glm::vec4> colors);
vector<glm::vec4> generateColor();


// this method will generate a random rectangle which is then stored in the data structure defined
// in validRectangle
validRectangle generateRectanle() {
    // setting up the random generator
    // actually need to generate point as vectors form
    random_device rd;
    mt19937 gen(rd());
    // generate a centre point randomly in the within given range..
    float centreX = (generate_canonical<float, 3>(gen))*2 - 1;
    float centreY = (generate_canonical<float, 3>(gen))*2 - 1;
    float height =  (generate_canonical<float, 3>(gen));
    float width =   (generate_canonical<float, 3>(gen));
    float halfH = height/2;
    float halfW = width/2;
    glm::vec3 centre = glm::vec3(centreX,centreY,0.0f);
    validRectangle rec;
    // the centre of the rectangle,height and width
    rec.centre = centre;
    rec.height = height;
    rec.width = width;
    //Top Right
    rec.firstPoint = glm::vec3(centreX + halfW,centreY+halfH,0.0f);
    //Bottom Right
    rec.secondPoint = glm::vec3(centreX + halfW,centreY-halfH,0.0f);
    //Bottom Left
    rec.thirdPoint = glm::vec3(centreX - halfW,centreY-halfH,0.0f);
    //Top Left
    rec.fouthPoint = glm::vec3(centreX - halfW,centreY+halfH,0.0f);
    return rec;
};

//setting up constants
const GLuint WIDTH = 1280, HEIGHT = 1280;
const GLuint RECNUM = 50;

void key_exit(GLFWwindow* window, int key, int scancode,int action,int mode) {
    
    // when a user presses the escape key, we set the windowShouldClose property to true
    
    if(key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}



// generate rectangle numbers of random colors
vector<glm::vec4> generateColor() {
    random_device rd;
    vector<glm::vec4> colors;
    float r,g,b,z;
    mt19937 gen(rd());
    for(int n = 0 ; n < RECNUM ; ++n) {
        r = (generate_canonical<float, 3>(gen));
        g = (generate_canonical<float, 3>(gen));
        b = (generate_canonical<float, 3>(gen));
        z = (generate_canonical<float, 3>(gen));
    glm::vec4 color = glm::vec4(r,g,b,z);
        colors.push_back(color);
    }
    return colors;
}


// Shaders
const GLchar* vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"color = ourColor;\n"
"}\n\0";

const GLchar* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"color = ourColor;\n"
"}\n\0";




int main(int argc, const char * argv[] ) {
    // setting up GLFW process
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // initialize the window with GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment1", nullptr, nullptr);

    //make the program listen to the key input
    glfwSetKeyCallback(window, key_exit);

    // Handle window not created successfully
    if(window == nullptr) {
        cout << "Failed to create a window" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    // setting up the GLEW
    glewInit();
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }
    
    //random colors vector
    
    vector<glm::vec4> colors = generateColor();
    
    for(auto i = colors.begin();i!=colors.end();++i) {
        cout << i->x << i->y << i->z << i->w << endl;
    }
    // test puporse for the gameloop
    auto j = colors.begin();
    
    // test case for the valid rectangle and store them in a vector;
    
    vector<validRectangle> rectangles;
    rectangles.push_back(generateRectanle());
    for(int i = RECNUM;i>=0;--i) {
        rectangles.push_back(generateRectanle());
    }
//    for(auto j = rectangles.begin(); j!=rectangles.end();++j) {
//        cout << "this is all the rectangles" << j->firstPoint.x << j->firstPoint.y << endl;
//    }
//    
    validRectangle rec = rectangles[0];
    validRectangle rec2 = rectangles[1];

    
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // compile the shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Next we attach the shader source code to the shader object and compile the shader:
    glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader);
    //check if the complication is sucessful
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //compile the fragmentShader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource2,NULL);
    glCompileShader(fragmentShader);
    // Shader Program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    // attch the shader to program created
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check if the link is succesfful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    
    //
    glUseProgram(shaderProgram);
    // we can delete them after the link
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,   // First Triangle
        1, 2, 3    // Second Triangle
    };
    
    // setting up objects
    GLuint VBOs[RECNUM],VAOs[RECNUM],EBOs[RECNUM];
    glGenVertexArrays(RECNUM,VAOs); // Generate multiple VAOs
    glGenBuffers(RECNUM,VBOs);
    glGenBuffers(RECNUM,EBOs);
    //===========================
    //First Rectangle
    //===========================
//    glBindVertexArray(VAOs[0]);
//    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(firstvertices), firstvertices, GL_STATIC_DRAW);
//    //EBO
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
//    glEnableVertexAttribArray(0);
//    glBindVertexArray(0);
    //===========================
    //Second Rectangle
    //===========================
//    glBindVertexArray(VAOs[1]);
//    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(secondvertices), secondvertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
//    glEnableVertexAttribArray(0);
//    glBindVertexArray(0);
    //===========================
    //More Rectangles
    //===========================
    int counter = 0;
    for(auto i = rectangles.begin();i!=rectangles.end();i++) {
        
        GLfloat vertices[] {
            i->firstPoint.x,i->firstPoint.y,0.0f,
            i->secondPoint.x,i->secondPoint.y,0.0f,
            i->thirdPoint.x,i->thirdPoint.y,0.0f,
            i->fouthPoint.x, i->fouthPoint.y,0.0f,
        };
        glBindVertexArray(VAOs[counter]);
        glBindBuffer(GL_ARRAY_BUFFER,VBOs[counter]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBOs[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        counter++;
//        cout << "test vertices built" << vertices[0] << vertices[1] << vertices[3] << endl;
    }
    
    

//    for(int i = 0; i <= RECNUM;i++) {
//        glBindVertexArray(VAOs[i]);
//        glBindBuffer(GL_ARRAY_BUFFER,VBOs[i]);
//        glBufferData(GL_ARRAY_BUFFER,sizeof(firstvertices),firstvertices,GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBOs[1]);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//        glEnableVertexAttribArray(0);
//        glBindVertexArray(0);
//        
//    }
    while(!glfwWindowShouldClose(window)) {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        //rendering commands here
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //test random color
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        // Draw our first rectangle
        glUseProgram(shaderProgram);
        //random color.
//        glUniform4f(vertexColorLocation,j->x,j->y,j->z,j->w);
//            j++;
//        
        // Create transformations
        glm::mat4 transform;
//        transform = glm::translate(transform, glm::vec3(0.2f, -0.2f, 0.0f));
//        transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 5.0f,glm::vec3(0.0f, 0.0f,  -1.0f));
//        transform = glm::scale(transform,glm::vec3(0.5f, 0.5f, 0.5f));
    
        // Get matrix's uniform location and set matrix
        GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        
        //drawing multiple
        for (int i = 0 ; i  < RECNUM ;i ++ ) {
            if(i < RECNUM/2 +1) {
                 transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.0f,glm::vec3(0.0f, 0.0f,  -1.0f));
                 glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            }
            else {
                 transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.0f,glm::vec3(0.0f, 0.0f,  1.0f));
                 glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            }
            glUniform4f(vertexColorLocation,j[i].x,j[i].y,j[i].z,j[i].w);
            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        }
        
        // drawing process
//        glBindVertexArray(VAOs[0]);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
////        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glUniform4f(vertexColorLocation,j->x,j->y,j->z,j->w);
//        glBindVertexArray(VAOs[1]);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    
    
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}





//test starting method

void key_start(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key== GLFW_KEY_S && action == GLFW_PRESS) {
        while(!glfwWindowShouldClose(window)) {
            
            
    }
        glfwSwapBuffers(window);

}
}

void render(GLuint shaderProgram,GLuint VAO, vector<glm::vec4> colors){
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();
    //rendering commands here
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //test random color
    GLint vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
    // Draw our first rectangle
    glUseProgram(shaderProgram);
    //random color.
    auto j = colors.begin();
    glUniform4f(vertexColorLocation,j->x,j->y,j->z,j->w);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    // Swap the screen buffers
}


