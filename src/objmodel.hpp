#ifndef _OBJMODEL_HPP_
#define _OBJMODEL_HPP_

#include <vector>
#include <utility>
#include "vector.hpp"
#include "model.hpp"

typedef std::vector< std::pair< std::string, std::string > > ObjUnknownsList;

void loadObjModel(const char* filename, Model& model, bool unify = true,
		ObjUnknownsList* unknowns = 0);

#endif
