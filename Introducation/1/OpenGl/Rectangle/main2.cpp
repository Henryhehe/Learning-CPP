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
vector<glm::vec4> generateColor();
const GLuint WIDTH = 640, HEIGHT = 640;
const GLuint RECNUM = 15;

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
        z = (generate_canonical<float,3>(gen));
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
const GLchar* fragmentShaderSource2 = "#version 330 core\n"
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
    
    auto j = colors.begin();
    
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
    
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    cout << vec.x << vec.y << vec.z << endl;
    
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
    
    //This is the gameloop
    
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
        glUniform4f(vertexColorLocation,j->x,j->y,j->z,j->w);
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
