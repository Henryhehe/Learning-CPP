//
//  main.cpp
//  Rectangle
//
//  Created by Chuanheng He on 2016-09-23.
//  Student Number:250825763
//  Email Address: che49@uwo.ca
//  Copyright © 2016 Chuanheng He. All rights reserved.

#include "Basic.h"

int start = 0;
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
    float halfH = height/RECSIZE;
    float halfW = width/RECSIZE;
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

// Handle window opens and closes
void key_exit(GLFWwindow* window, int key, int scancode,int action,int mode) {
    
    // when a user presses the escape key, we set the windowShouldClose property to true
    if(key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key== GLFW_KEY_S && action == GLFW_PRESS) {
            start = 1;
    }
    if(key== GLFW_KEY_R && action == GLFW_PRESS) {
        start = 0;
    }
}
// generate rectangle numbers of random colors
vector<glm::vec4> generateColor() {
    random_device rd;
    vector<glm::vec4> colors;
    float r,g,b,z;
    mt19937 gen(rd());
    for(int n = 0 ; n < RECNUM ; ++n) {
        r = (generate_canonical<float, 3>(gen)) + 0.5;
        g = (generate_canonical<float, 3>(gen));
        b = (generate_canonical<float, 3>(gen)) + 0.5;
        z = (generate_canonical<float, 3>(gen));
    glm::vec4 color = glm::vec4(r,g,b,z);
        colors.push_back(color);
    }
    return colors;
}
// Shaders
// reverse * transform * vec4(position.x, position.y, position.z, 1.0);\n"
const GLchar* vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 transform;\n"
"uniform mat4 reverse;\n"
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
    //color iterator
    auto j = colors.begin();
    //randome rectangles vector
    vector<validRectangle> rectangles;
    rectangles.push_back(generateRectanle());
    for(int i = RECNUM;i>=0;--i) {
        rectangles.push_back(generateRectanle());
    }
    

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
    // draw indices
    GLuint indices[] = {
        0, 2, 3,
        1, 2, 3
    };
    
//        GLuint indices[] = {
//            3, 3, 2,
//            1, 2, 3
//        };
    

//    
    // setting up buffer and attribute objects
    GLuint VBOs[RECNUM],VAOs[RECNUM],EBOs[RECNUM];
    glGenVertexArrays(RECNUM,VAOs); // Generate multiple VAOs
    glGenBuffers(RECNUM,VBOs);
    glGenBuffers(RECNUM,EBOs);
    //testing more random
    std::random_device rd2; // obtain a random number from hardware
    std::mt19937 eng(rd2()); // seed the generator
    std::uniform_real_distribution<> distr_1(-0.25, 0.25);
    std::uniform_real_distribution<> distr_2(-0.25, 0.25 );// define the range
    float xChange = distr_1(eng);
    float yChange = distr_2(eng);
    float zChange = distr_1(eng);
    float wChange = distr_1(eng);
    
    //generate rectangles store them into VAOs and VBOs
    int counter = 0;
    for(auto i = rectangles.begin();i!=rectangles.end();i++) {
    //originally z should be 1.0f but now we're making it random
    // and the x and y is just the point values
        GLfloat vertices[] {
            i->firstPoint.x + xChange,i->firstPoint.y + xChange,xChange,
            i->secondPoint.x + yChange,i->secondPoint.y + yChange,yChange,
            i->thirdPoint.x + zChange,i->thirdPoint.y + zChange,zChange,
            i->fouthPoint.x + wChange, i->fouthPoint.y + wChange,wChange,
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
    }
        vector<glm::mat4> transforms;
      // generate random transform matrix for each rectangles and store them into a vector so
      // that we'd be to change them and acess them
    for(int j = 0 ; j < RECNUM;j++) {
        glm::mat4 transform;
        transforms.push_back(transform);
    };
    
   //drawing process
    while(!glfwWindowShouldClose(window)) {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        //rendering commands here
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //test random color
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 eng(rd()); // seed the generator
        std::uniform_int_distribution<> distr(1, 2000);
        std::uniform_int_distribution<> distr2(1, 2000);// define the range
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        // Draw our first rectangle
        glUseProgram(shaderProgram);
        // Create transformations
        // glm::mat4 transform;
        glm::mat4 reverse;
        // Get matrix's uniform location and set matrix
        GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        GLint reverseLoc = glGetUniformLocation(shaderProgram,"reverse");
        //drawing multiples
        for (int i = 0 ; i  < RECNUM ;i ++ ) {
            if(start) {
            glm::mat4 transform = transforms[i];
                int xscale = distr(eng);
                int yscale = distr2(eng);
                if(i%2==0) {
        transforms[i]=  glm::scale(transform,glm::vec3(1.0f - cos((GLfloat)glfwGetTime())/xscale/10,1.0f,1.0f));
        transform = transforms[i];
        transforms[i] = glm::rotate(transform,xscale*1.0f/50000,glm::vec3(yscale/100.0,yscale/100.0 ,sin(yscale/100.0)));
        transform = transforms[i];
        transforms[i] = glm::translate(transform, glm::vec3(cos(xscale)/50*sin(yscale/100),cos(xscale)/50*sin(yscale/100),cos(xscale)/100*sin(yscale/100)));
                    
                }
                else {
        transforms[i]=  glm::scale(transform,glm::vec3(1.0f,1.0f - cos((GLfloat)glfwGetTime())/xscale/10,1.0f));
        transform = transforms[i];
        transforms[i] = glm::rotate(transform,xscale*1.0f/50000,glm::vec3(-yscale/100.0,-yscale/100.0,-yscale/100.0));
        transform = transforms[i];
        transforms[i] = glm::translate(transform, glm::vec3(sin(xscale)/30*cos(yscale/40),sin(xscale)/20*cos(yscale/10),0.0f));
                }
            }
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transforms[i]));
            glUniformMatrix4fv(reverseLoc,1,GL_FALSE,glm::value_ptr(reverse));
            glUniform4f(vertexColorLocation,j[i].x,j[i].y,j[i].z,j[i].w);
            glBindVertexArray(VAOs[i]);
            glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(RECNUM, VAOs);
    glDeleteBuffers(RECNUM, VBOs);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}


