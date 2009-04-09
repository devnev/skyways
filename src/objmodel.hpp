#ifndef _OBJMODEL_HPP_
#define _OBJMODEL_HPP_

#include <vector>
#include "vector.hpp"

void loadObjModel(const char* filename, std::vector< Vector3 >& vertices, std::vector< Triangle >& faces);

#endif
