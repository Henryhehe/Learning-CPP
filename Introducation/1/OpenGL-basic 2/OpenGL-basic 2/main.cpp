//
//  main.cpp
//  OpenGL-basic 2
//
//  Created by Chuanheng He on 2016-09-13.
//  Copyright © 2016 Chuanheng He. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "main.h"
#include "Shader.h"


using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Function to create_texture

bool initTexture()
{
    gli::texture2d Texture(gli::load((getDataDirectory() + TEXTURE_DIFFUSE)));
    if(Texture.empty())
        return false;
    
    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
    
    glGenTextures(1, &TextureName);
    
    // Default unpack alignment of 4 is ok, we have 32 bit per pixel.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &Format.Swizzles[0]);
    
    glTexImage2D(GL_TEXTURE_2D, 0,
                 Format.Internal,
                 GLsizei(Texture[0].extent().x), GLsizei(Texture[0].extent().y),
                 0,
                 Format.External, GL_UNSIGNED_INT_10_10_10_2,//Format.Type,
                 Texture[0].data());
    
    return true;
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
    ofstream Newfile;
    Newfile.open("newfile.txt");
    Newfile << "Hello ";
    
    
    Shader ourShader("ProjectionShader.sd", "fragment.fg");

//     test the liner algerbra libary 
//    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//    glm::mat4 trans;
//    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//    vec = trans * vec;
//    cout << vec.x << vec.y << vec.z << endl;

    
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
    // Set up vertex data (and buffer(s)) and attribute pointers
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions          // Texture Coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    
    GLuint VBO, VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0); // Unbind VAO
    
    // Load and create a texture
    GLuint texture1;
    GLuint texture2;
//    ====================
//     Texture 1
//     ====================
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     Load, create texture and generate mipmaps
    int width2, height2;
    unsigned char* image = gli::load("container.jpg").data();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    // ===================
    // Texture 2
    // ===================
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
  
    
    glRotated;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // Bind Textures using texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
        
        // Activate shader
        ourShader.Use();
        
        // Create transformations
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}