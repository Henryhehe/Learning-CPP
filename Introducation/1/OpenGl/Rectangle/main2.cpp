//
//  main.cpp
//  Rectangle
//
//  Created by Chuanheng He on 2016-09-12.
//  Copyright © 2016 Chuanheng He. All rights reserved.
//

//
//  Rectangle.cpp
//  OpenGl
//
//  Created by Chuanheng He on 2016-09-12.
//  Copyright © 2016 Chuanheng He. All rights reserved.
//
#include "Basic.h"

using namespace std;

//Shader language GLSL(OpenSL shading language, which is a vertex shader
//ext we declare all the input vertex attributes in the vertex shader with the in keyword. Right now we only care about position data so we only need a single vertex attribute. GLSL has a vector datatype that contains 1 to 4 floats based on its postfix digit. Since each vertex has a 3D coordinate we create a vec3 input variable with the name position. We also specifically set the location of the input variable via layout (location = 0) and you'll later see that why we're going to need that location.
// Function prototypes
void key_callback2(GLFWwindow* window, int key, int scancode, int action, int mode);
//const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.3f, 0.1f, 0.1f, 1.0f);\n"
"}\n\0";

void key_callback2(GLFWwindow* window, int key, int scancode,int action,int mode) {
    
    // when a user presses the escape key, we set the windowShouldClose property to true
    
    if(key == GLFW_KEY_0 && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    
}


int main(int argc, const char * argv[] ) {
    // setting up GLFW process
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Learning", nullptr, nullptr);
    
    glfwSetKeyCallback(window, key_callback2);
    
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
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    glViewport(0, 0, width, height);
    
    
    // compile the shader
    
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    //Next we attach the shader source code to the shader object and compile the shader:
    glShaderSource(vertexShader, 1, &vertexShaderSource1, NULL);
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
    
    
    // Specified how OpenGl should interprate the vertex data
    
    //    // 0. Copy our vertices array in a buffer for OpenGL to use
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //    // 1. Then set the vertex attributes pointers
    //    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //    glEnableVertexAttribArray(0);
    //    // 2. Use our shader program when we want to render an object
    //    glUseProgram(shaderProgram);
    //    // 3. Now draw the object
    //    someOpenGLFunctionThatDrawsOurTriangle();
    
    // VAO
    // Set up vertex data (and buffer(s)) and attribute pointers
    // use element buffer object
    // and use index drawing
    GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,   // First Triangle
        1, 2, 3    // Second Triangle
    };
    
    
    GLuint VBO, VAO,EBO;
    
    glGenVertexArrays(1, &VAO);
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // Call to enable the function
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
    
    
    
    while(!glfwWindowShouldClose(window)) {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        //rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw our first rectangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // Swap the screen buffers
        glfwSwapBuffers(window); }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    
    
    return 0;
}
