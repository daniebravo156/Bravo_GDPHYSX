#define TINYOBJLOADER_IMPLEMENTATION
#include "headers/ModelLoader.h"
#include <iostream>

// 3d model loading helper function
bool LoadMy3DModel(std::string filePath, tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes) {
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    // make sure the path is correct and the file exists
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str());
    if (!ret) {
        std::cerr << "Failed to load asset " << filePath << ": " << err << std::endl;
    }
    return ret;
}