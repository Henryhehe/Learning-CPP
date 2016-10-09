//
//  main.cpp
//  OpenGL_light
//
//  Created by Chuanheng He on 2016-10-04.
//  Copyright © 2016 Chuanheng He. All rights reserved.
//

#include <iostream>
#include "main.h"
#include "Shader.h"
#include "Camera.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;
bool    keys[1024];

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Shaders
// reverse * transform * vec4(position.x, position.y, position.z, 1.0);\n"
const GLchar* vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"}\0";

const GLchar* vertexShaderSource3 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 normal;\n"
"out vec3 Normal;\n"
"out vec3 FragPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"FragPos = vec3(model * vec4(position, 1.0f));\n"
"Normal = mat3(transpose(inverse(model))) * normal;\n"
"}\0";

const GLchar* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec3 objectColor;\n"
"uniform vec3 lightColor;\n"
"void main()\n"
"{\n"
"color = vec4(lightColor * objectColor, 1.0f);\n"
"}\0";

const GLchar* fragmentLight = "#version 330 core\n"
"out vec4 color;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 objectColor;\n"
"uniform vec3 lightColor;\n"
"void main()\n"
"{\n"
"float ambientStrength = 0.1f;\n"
"vec3 ambient = ambientStrength * lightColor;\n"
"vec3 norm = normalize(Normal);\n"
"vec3 lightDir = normalize(lightPos - FragPos);\n"
"float diff = max(dot(norm, lightDir), 0.0);\n"
"vec3 diffuse = diff * lightColor;\n"
"vec3 result = (ambient + diffuse) * objectColor;\n"
"color = vec4(result, 1.0f);\n"
"}\0";

const GLchar* fragmentLight2 = "#version 330 core\n"
"out vec4 color;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 objectColor;\n"
"uniform vec3 lightColor;\n"
"void main()\n"
"{\n"
"float ambientStrength = 0.2f;\n"
"vec3 ambient = ambientStrength * lightColor;\n"
"vec3 norm = normalize(Normal);\n"
"vec3 lightDir = normalize(lightPos - FragPos);\n"
"float diff = max(dot(norm, lightDir), 0.0);\n"
"vec3 diffuse = diff * lightColor;\n"
"float specularStrength = 0.5f;\n"
"vec3 viewDir = normalize(viewPos - FragPos);\n"
"vec3 reflectDir = reflect(-lightDir, norm);\n"
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);\n"
"vec3 specular = specularStrength * spec * lightColor;\n"
"vec3 result = (ambient + diffuse+ specular) * objectColor;\n"
"color = vec4(result, 1.0f);\n"
"}\0";

const GLchar* fragmentLight3 = "#version 330 core\n"
"struct Material {\n"
"vec3 ambient;\n"
"vec3 diffuse;\n"
"vec3 specular;\n"
"float shininess;\n"
"};\n"
"struct Light { \n"
"vec3 position;\n"
"vec3 ambient;\n"
"vec3 diffuse;\n"
"vec3 specular;\n"
"};\n"

"out vec4 color;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"uniform vec3 viewPos;\n"
"uniform Light light;\n"
"uniform Material material;\n"
"void main()\n"
"{\n"
"vec3 ambient = light.ambient * material.ambient;\n"
"vec3 norm = normalize(Normal);\n"
"vec3 lightDir = normalize(light.position - FragPos);\n"
"float diff = max(dot(norm, lightDir), 0.0);\n"
"vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
"vec3 viewDir = normalize(viewPos - FragPos);\n"
"vec3 reflectDir = reflect(-lightDir, norm);\n"
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
"vec3 specular = light.specular * (spec * material.specular);\n"
"vec3 result = ambient + diffuse + specular;\n"
"color = vec4(result, 1.0f);\n"
"}\0";

const GLchar* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.9f);\n"
"}\0";

int main(int argc, const char * argv[]) {
    // setting up GLFW process
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Learning", nullptr, nullptr);
    
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(window == nullptr) {
        cout << "Failed to create a window" << endl;
        glfwTerminate();
        return -1;
    }
    
    glewExperimental = GL_TRUE;
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
    
    glEnable(GL_DEPTH_TEST);

    
   // Build and compile our shader program
    //compile the fragmentShader
    
    // compile the shader
    GLint success;
    GLchar infoLog[512];
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Next we attach the shader source code to the shader object and compile the shader:
    glShaderSource(vertexShader, 1, &vertexShaderSource3, NULL);
    glCompileShader(vertexShader);
    
    GLuint LightingShader;
    LightingShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(LightingShader,1,&fragmentLight3,NULL);
    glCompileShader(LightingShader);
    // Shader Program
    GLuint LightingProgram;
    LightingProgram = glCreateProgram();
    // attch the shader to program created
    glAttachShader(LightingProgram,vertexShader);
    glAttachShader(LightingProgram,LightingShader);
    glLinkProgram(LightingProgram);
    
    // check if the link is succesfful
    glGetProgramiv(LightingProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(LightingProgram, 512, NULL, infoLog);
    }
    
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource3,NULL);
    glCompileShader(fragmentShader);
    // Shader Program
    GLuint LampProgram;
    LampProgram = glCreateProgram();
    // attch the shader to program created
    glAttachShader(LampProgram,vertexShader);
    glAttachShader(LampProgram,fragmentShader);
    glLinkProgram(LampProgram);
    
//    ofstream myfile ("vertexShader.vts");
//    myfile <<  vertexShaderSource2;
//    myfile.close();
//    
//    ofstream myfile2("frament.fg");
//    myfile2 << fragmentShaderSource2;
//    myfile.close();
//    
//    ofstream myfile3("lamp.fg");
//    myfile3 << fragmentShaderSource3;
//    myfile3.close();
    
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices2[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // First, set the container's VAO (and VBO)
    GLuint VBO, containerVAO,VBO2;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(containerVAO);
    // Position attribute and normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindVertexArray(lightVAO);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    // Game loop
    GLint viewPosLoc = glGetUniformLocation(LightingProgram, "viewPos");
    glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
  
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();
        
        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use cooresponding shader when setting uniforms/drawing objects
        glUseProgram(LightingProgram);
     
        GLint lightPosLoc = glGetUniformLocation(LightingProgram, "lightPos");
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        
        // Set lights properties
        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        
        glm::vec3 diffuseColor = lightColor * glm::vec3(2.5f); // Decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(2.2f); // Low influence
        glUniform3f(glGetUniformLocation(LightingProgram, "light.ambient"),  ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(glGetUniformLocation(LightingProgram, "light.diffuse"),  diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(glGetUniformLocation(LightingProgram, "light.specular"), 1.0f, 1.0f, 1.0f);
        
        GLint matAmbientLoc  = glGetUniformLocation(LightingProgram, "material.ambient");
        GLint matDiffuseLoc  = glGetUniformLocation(LightingProgram, "material.diffuse");
        GLint matSpecularLoc = glGetUniformLocation(LightingProgram, "material.specular");
        GLint matShineLoc    = glGetUniformLocation(LightingProgram, "material.shininess");
        
        glUniform3f(matAmbientLoc,  1.0f, 0.5f, 0.31f);
        glUniform3f(matDiffuseLoc,   1.0f, 0.5f, 0.31f);
        glUniform3f(matSpecularLoc,  0.5f, 0.5f, 0.5f);
        glUniform1f(matShineLoc,    32.0f);
        // Create camera transformations
        
        
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(LightingProgram, "model");
        GLint viewLoc  = glGetUniformLocation(LightingProgram,  "view");
        GLint projLoc  = glGetUniformLocation(LightingProgram,  "projection");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // Draw the container (using container's vertex attributes)
        glBindVertexArray(containerVAO);
        glm::mat4 model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        // Also draw the lamp object, again binding the appropriate shader
        glUseProgram(LampProgram);
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(LampProgram, "model");
        viewLoc  = glGetUniformLocation(LampProgram, "view");
        projLoc  = glGetUniformLocation(LampProgram, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;

}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
