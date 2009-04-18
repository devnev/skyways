//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#ifndef _OBJMODEL_HPP_
#define _OBJMODEL_HPP_

#include <vector>
#include <utility>
#include "vector.hpp"
#include "model.hpp"

typedef std::vector< std::pair< std::string, std::string > > ObjUnknownsList;

void loadObjModel(const char* filename, Model& model, bool unify = true,
		ObjUnknownsList* unknowns = 0, const char * expectedUnknowns = 0);

#endif
