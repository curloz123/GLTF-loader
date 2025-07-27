#include "gltLoader.h"
#include <iterator>
#include <string>


GLTloader::GLTloader(const std::string &path, const std::string &type)
{
    if(!loadModel(path, type))
    {
        std::cout<<"Could not load model"<<std::endl;
    }
}

bool GLTloader::loadModel(const std::string &path, const std::string &type)
{
    //Holds the whole scene
    tinygltf::Model model;

    //The file reader
    tinygltf::TinyGLTF loader;

    //Check for error and warnings
    std::string error, warning;

    //Check file type
    bool ret;
    if(type == "gltf")
    {
        //load file into memory
        ret = loader.LoadASCIIFromFile(&model, &error, &warning, path);
    }
    else if(type == "glb")
    {
        //load file into memory
        ret = loader.LoadBinaryFromFile(&model, &error, &warning, path);
    }
    else
    {
        std::cout<<"ERROR->invalid file type"<<std::endl;
        return false;
    }

    //Check if file loaded properly
    if(!ret) return false;

    //Check for any errors and warnings
    if(!error.empty()) std::cout<<"ERROR-> "<<error<<std::endl;
    if(!warning.empty()) std::cout<<"WARNING-> "<<warning<<std::endl;


    //looping through and working on all the meshes
    for(const auto &mesh : model.meshes)
    {
        //looping over all its primitives
        for(const auto &prim : mesh.primitives)
        {
            gltMesh glMesh;
            std::vector<float> vertexData;
            std::vector<GLuint> indices;


            //Read the primitives
            
            //Accessor
            const auto &posAccesor = model.accessors[prim.attributes.find("POSITION")->second];
            //Buffer view
            const auto &posView = model.bufferViews[posAccesor.bufferView];
            //Buffer
            const auto &posBuffer = model.buffers[posView.buffer];



            //in a .gltf file:
            //This vertex data is stored deep inside a big byte blob.
            //There is no direct float* or std::vector<float> — it's just binary bytes.
            //So we must extract the required float* from it
            //So we must extract it and reinterpret it.

            //Think of it like this
            //buffer.data = the whole book
            //byteOffset = skip to chapter 3 (positions)
            //accessor.byteOffset = skip 20 words into the chapter
            //reinterpret_cast<const float*> = start reading those words as float numbers, not raw bytes

            //We perform it using this shit looking line
            const float *posData = reinterpret_cast<const float*>(&posBuffer.data[posView.byteOffset + posAccesor.byteOffset]);



            //Push vertex date
            size_t vertexCount = posAccesor.count;
            vertexData.insert(vertexData.end(),posData,posData + vertexCount * 3);



            //Reading indices
            if (prim.indices >=0)
            {
                const auto &idAccesor = model.accessors[prim.indices];
                const auto &idView = model.bufferViews[idAccesor.bufferView];
                const auto &idBuffer = model.buffers[idView.buffer];

                //retrieving float data
                const unsigned char* data = idBuffer.data.data() + idView.byteOffset + idAccesor.byteOffset;
                
                //Type casting it to proper format
                switch(idAccesor.componentType)
                {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE :
                        {
                            const uint8_t *idData = reinterpret_cast<const uint8_t*>(data);
                            for(int i=0; i<idAccesor.count; ++i)
                            {
                                indices.push_back(idData[i]);
                            }
                            break;
                        }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT :
                        {
                            const uint16_t *idData = reinterpret_cast<const uint16_t*>(data);
                            for(int i=0; i<idAccesor.count; ++i)
                            {
                                indices.push_back(idData[i]);
                            }
                            break;
                        }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT :
                        {
                            const uint32_t *idData = reinterpret_cast<const uint32_t*>(data);
                            for(int i=0; i<idAccesor.count; ++i)
                            {
                                indices.push_back(idData[i]);
                            }
                            break;
                        }
                    default :
                        std::cout<<"Unsupported index type"<<std::endl;



                }

            }
        

            //Setting up opengl buffers
            glGenVertexArrays(1,&glMesh.VAO);
            glGenBuffers(1, &glMesh.VBO);
            glGenBuffers(1, &glMesh.EBO);

            glBindVertexArray(glMesh.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, glMesh.VBO);
            glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);


            glBindVertexArray(0);

            //Passing number of indices to our mesh struct
            glMesh.indexCount = indices.size();

            //pushing our final big mesh
            meshes.push_back(glMesh);

        }
    
    }

    std::cout<<"Loaded model properly"<<std::endl;
    return true;
}


void GLTloader::Draw(Shader &shader)
{
    //Drawing model
    for(auto &mesh : meshes)
    {
        mesh.Draw(shader);
    }
}




