#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>
#include <vector>
#include "tiny_obj_loader.h"

// 3d model loading helper function
bool LoadMy3DModel(std::string filePath, tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes);

#endif