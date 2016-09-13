//
//  main.cpp
//  OpenGl
//
//  Created by Chuanheng He on 2016-09-09.
//  Copyright © 2016 Chuanheng He. All rights reserved.
//
#include "Basic.h"

using namespace std;

//Shader language GLSL(OpenSL shading language, which is a vertex shader
//ext we declare all the input vertex attributes in the vertex shader with the in keyword. Right now we only care about position data so we only need a single vertex attribute. GLSL has a vector datatype that contains 1 to 4 floats based on its postfix digit. Since each vertex has a 3D coordinate we create a vec3 input variable with the name position. We also specifically set the location of the input variable via layout (location = 0) and you'll later see that why we're going to need that location.
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
//chaning color green to black
const GLchar* vertexShaderChange = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

// this one wirth color
const GLchar* vertexShaderColor = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\0";

//combine with vertexShaderColor

const GLchar* fragmentShaderColor = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";

//chaning color green to black
const GLchar* fragmentShaderChange = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"color = ourColor;\n"
"}\n\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.2f, 0.3f, 1.0f, 1.0f);\n"
"}\n\0";

const GLchar* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.1f, 0.3f, 0.3f, 1.0f);\n"
"}\n\0";


void key_callback(GLFWwindow* window, int key, int scancode,int action,int mode) {
    
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
    
    glfwSetKeyCallback(window, key_callback);
    
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
    
    GLuint vertexShader,vertexChange,vertexColor;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    vertexChange = glCreateShader(GL_VERTEX_SHADER);
    vertexColor = glCreateShader(GL_VERTEX_SHADER);
    
//Next we attach the shader source code to the shader object and compile the shader:
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    glShaderSource(vertexChange, 1, &vertexShaderChange, NULL);
    glCompileShader(vertexChange);
    
    glShaderSource(vertexColor,1,&vertexShaderColor,NULL);
    glCompileShader(vertexColor);
    
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
    GLuint fragmentShader,fragmentShader2,fragmentChange,fragmentColor;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2,1,&fragmentShaderSource2,NULL);
    glCompileShader(fragmentShader2);
    
    fragmentChange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentChange,1,&fragmentShaderChange,NULL);
    glCompileShader(fragmentChange);
    
    fragmentColor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentColor,1,&fragmentShaderColor,NULL);
    glCompileShader(fragmentColor);
    
    
    
// Shader Program
    
    GLuint shaderProgram,shaderProgram2,shaderChange,shaderColor;
    shaderProgram = glCreateProgram();
    shaderProgram2 = glCreateProgram();
    shaderChange = glCreateProgram();
    shaderColor = glCreateProgram();
    
    
// attch the shader to program created
    
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    
    glAttachShader(shaderProgram2,vertexShader);
    glAttachShader(shaderProgram2,fragmentShader2);
    glLinkProgram(shaderProgram2);
    
    
    glAttachShader(shaderChange,vertexChange);
    glAttachShader(shaderChange,fragmentChange);
    glLinkProgram(shaderChange);
    
    glAttachShader(shaderColor,vertexColor);
    glAttachShader(shaderColor,fragmentColor);
    glLinkProgram(shaderColor);
    
// check if the link is succesfful
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
//
    
// we can delete them after the link
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexChange);
    glDeleteShader(fragmentChange);
    

    
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
    GLfloat vertices[] = {
        -0.9f, -0.5f, 0.0f,  // Left
        -0.0f, -0.5f, 0.0f,  // Right
        -0.45f, 0.5f, 0.0f,
    };
    
    GLfloat vertices2[] = {
        
        0.0f, -0.5f, 0.0f,  // Left
        0.9f, -0.5f, 0.0f,  // Right
        0.45f, 0.5f, 0.0f   // Top
    };
    
    GLfloat verticescolor[] = {
        
        // Positions         // Colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top
        
    };
    
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
   
    
    glGenBuffers(1, &VBO);
   

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticescolor), verticescolor, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
//    Change color attribute array
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    
    
    
//    
//    glBindVertexArray(VAO2);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO2);	// And a different VBO
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // Because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out.
//    glEnableVertexAttribArray(0);
//    glBindVertexArray(0);
    
    
    
    while(!glfwWindowShouldClose(window)) {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
     
        
        //rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw our first triangle
        glUseProgram(shaderColor);
        
//        // Update the uniform color
//        GLfloat timeValue = glfwGetTime();
//        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
//        GLint vertexColorLocation = glGetUniformLocation(shaderChange, "ourColor");
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
//        
        
         glBindVertexArray(VAO);
        
         glDrawArrays(GL_TRIANGLES, 0, 3);
//        
//        glBindVertexArray(VAO2);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window); }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
//    glDeleteVertexArrays(1, &VAO2);
//    glDeleteBuffers(1, &VBO2);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
    }


//////////////////////////////Smart way/////////////

//GLuint VBOs[2], VAOs[2];
//glGenVertexArrays(2, VAOs); // We can also generate multiple VAOs or buffers at the same time
//glGenBuffers(2, VBOs);
//// ================================
//// First Triangle setup
//// ===============================
//glBindVertexArray(VAOs[0]);
//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
//glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
//glEnableVertexAttribArray(0);
//glBindVertexArray(0);
//// ================================
//// Second Triangle setup
//// ===============================
//glBindVertexArray(VAOs[1]);	// Note that we bind to a different VAO now
//glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// And a different VBO
//glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); // Because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out.
//glEnableVertexAttribArray(0);
//glBindVertexArray(0);
//
//// Game loop
//while (!glfwWindowShouldClose(window))
//{
//    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
//    glfwPollEvents();
//    
//    // Render
//    // Clear the colorbuffer
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    // Activate shader (same shader for both triangles)
//    glUseProgram(shaderProgram);
//    // Draw first triangle using the data from the first VAO
//    glBindVertexArray(VAOs[0]);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    // Then we draw the second triangle using the data from the second VAO
//    glBindVertexArray(VAOs[1]);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);
//    
//    // Swap the screen buffers
//    glfwSwapBuffers(window);
//}
//// Properly de-allocate all resources once they've outlived their purpose
//glDeleteVertexArrays(2, VAOs);
//glDeleteBuffers(2, VBOs);
//// Terminate GLFW, clearing any resources allocated by GLFW.
//glfwTerminate();
//return 0;
//}

