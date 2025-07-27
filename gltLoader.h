/*********************************************/
/*********************************************/
/**This file is important for loading models**/
/************of type gltf and glb*************/
/*********************************************/
/*********************************************/



#pragma once


#include "tiny_gltf.h"
#include "include/glad/glad.h"
#include "shaders.h"
#include <vector>
#include <iostream>


struct gltMesh
{
    GLuint VAO,VBO,EBO;
    GLsizei indexCount;

    void Draw(Shader &shader)
    {
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,indexCount, GL_UNSIGNED_INT,0);
        
    }   
};

class GLTloader
{
    public:
        //OpenGL buffers
        GLuint VAO,VBO,EBO;

        //constructor that learns the file location
        GLTloader(const std::string &path, const std::string &type);

        //A list of all loaded meshes
        std::vector<gltMesh> meshes;

        //load the .glt* file Returns true if successful
        bool loadModel(const std::string &path , const std::string &type);

        //Draws the model
        void Draw(Shader &shader);

};


