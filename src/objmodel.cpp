#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "vector.hpp"
#include "range.hpp"
#include "objmodel.hpp"

static bool parseFacePoint(const char* str, size_t& v, size_t& vt, size_t& vn)
{
	// parse 3 numbers from string, seperated by slashes.
	// missing numbers are set to 0, e.g. "3//2"->3,0,2
	// only 1 slash means only vertex and texture
	// 0 slashes means only vertex
	v = vt = vn = 0;
	for ( ; (*str) && (*str) != '/'; ++str)
		v = v*10 + ((*str)-'0');
	if (!(*str)) return true; else ++str;
	for ( ; (*str) && (*str) != '/'; ++str)
		vt = vt*10 + ((*str)-'0');
	if (!(*str)) return true; else ++str;
	for ( ; (*str); ++str)
		vn = vn*10 + ((*str)-'0');
	return true;
}

void loadObjModel(const char* filename, Model& model)
{
	// Loading the mesh.
	// Lines begin with either '#' to indicate a comment, 'v' to indicate a vertex
	//	('vt' indicating skin vertex and 'vn' a vector normal) or 'f' to indicat a face.
	// ** Comments are ignored.
	// ** Vertex indices (in 'f' lines) are one based, so they start with one and not zero.
	// ** Faces may have three or four indices so they must be triangles or quads.
	// ** Any other tag in the file is ignored.

	std::vector< Vector3 >& vertices = model.vertices;
	std::vector< Vector3 >& normals = model.normals;
	std::vector< Face >& faces = model.faces;

	// open file
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::runtime_error(std::string("Unable to open OBJ file ") + filename);

	std::string line;
	bool unknownWarning = false;
	Range<size_t> vertexRange = makeInvalidRange<size_t>()
#if 0
		, textureRange = makeInvalidRange<size_t>()
#endif
		, normalRange = makeInvalidRange<size_t>()
	;
	std::vector< Vector3 > _vertices;
	std::vector< Vector3 > _normals;
	std::vector< Face > _faces;

	Range<double> xrange, yrange, zrange;

	while (getline(file, line))
	{
		std::istringstream iss(line);
		std::string cmd;
		// ignore empty lines and lines with '#' as first non-space char
		if (!(iss >> cmd) || cmd[0] == '#')
			continue;
		else if (cmd == "vn")
		{
			double x, y, z;
			if (!(iss >> x >> y >> z))
				throw std::runtime_error("Invalid vertex normal: " + line);
			_normals.push_back(Vector3(x, y, z));
		}
		else if (cmd == "vt")
		{
#if 0
			double tx, ty;
			if (!(iss >> tx >> ty))
				throw std::runtime_error("Invalid vertex texcoord: " + line);
			m_texpoints.push_back(SkinVertex(tx, ty));
#endif
		}
		else if (cmd == "v")
		{
			double x, y, z;
			if (!(iss >> x >> y >> z))
				throw std::runtime_error("Invalid vertex: " + line);
			xrange.include(x); yrange.include(y), zrange.include(z);
			_vertices.push_back(Vector3(x, y, z));
		}
		else if (cmd == "f")
		{
			std::string fp[3];
			if (!(iss >> fp[0] >> fp[1] >> fp[2]))
				throw std::runtime_error("Invalid face: " + line);
			Face face; size_t it; // it: dummy
			for (size_t i = 0; i < 3; ++i)
			{
				// parse indices from triplets
				// note that even though indices are 1-based in the file, they
				// are not decremented here. this is so the min/max ranges can
				// be used for validation
				if (!parseFacePoint(fp[i].c_str(), face.vertices.ix[i], it, face.normals.ix[i]))
					throw std::runtime_error("Invalid face: " + line);
				vertexRange.include(face.vertices.ix[i]);
#if 0
				textureRange.include(face.it[i]);
#endif
				normalRange.include(face.normals.ix[i]);
			}
			// some obj generators create "triangles" with more than one
			// identical edge index (crashes normal calculation) - ingore those
			if (face.valid())
				_faces.push_back(face);
			if (iss >> fp[0]) // theres a fourth corner -> it's a quad
			{
				// the second tri in the quad has corners (old 2, new, old 0)
				using std::swap;
				swap(face.vertices.ix[0], face.vertices.ix[2]);
				if (!parseFacePoint(fp[0].c_str(), face.vertices.ix[1], it, face.normals.ix[1]))
					throw std::runtime_error("Invalid face: " + line);
				if (face.valid())
					_faces.push_back(face);
			}
		}
		else
		{
			if (!unknownWarning)
			{
				std::cout << "Unknown token \"" << cmd << "\" encountered. (Additional warnings suppressed.)\n";
				unknownWarning = true;
			}
		}
	}
	file.close();

	// validate loaded object
	if (_vertices.size() == 0) // must have vertices
		throw std::runtime_error("Bad OBJ file: no vertices");
	if (_faces.size() == 0) // must have faces
		throw std::runtime_error("Bad OBJ file: no faces");
	if (vertexRange.min == 0)
		throw std::runtime_error("Bad OBJ file: faces with invalid vertex indices");
	if (vertexRange.max > _vertices.size())
		throw std::runtime_error("Bad OBJ file: out-of-range vertex indices");
#if 0
	if (textureRange.max > m_texpoints.size())
		throw std::runtime_error("Bad OBJ file: out-of-range texture indices");
	if (textureRange.min == 0 && textureRange.max != 0)
		throw std::runtime_error("Bad OBJ file: mixed texture usage on faces");
#endif
	if (normalRange.max > _normals.size())
		throw std::runtime_error("Bad OBJ file: out-of-range normal indices");
	if (normalRange.min == 0 && normalRange.max != 0)
		throw std::runtime_error("Bad OBJ file: mixed normal usage on faces");

	if (normalRange.max == 0)
		_normals.clear();

#if 0
	// this is currently hardcoded, it may be parametrized later on
	bool recalculateNormals = false;

	// if there are no normals, force normal recalculation
	if (m_normals.size() == 0 || normalRange.max == 0)
		recalculateNormals = true;

	if (recalculateNormals)
	{
		// recalculate normals, one normal per vertex
		m_normals.clear();
		m_normals.resize(m_vertices.size(), Vector3(0, 0, 0));
		for (std::vector<Face>::iterator face = m_faces.begin(); face != m_faces.end(); ++face)
		{
			const Vector3 faceNormal((m_vertices[face->i[1]-1] - m_vertices[face->i[0]-1]).cross(m_vertices[face->i[2]-1] - m_vertices[face->i[0]-1]).normalized());
			m_normals[face->i[0]-1] += faceNormal;
			m_normals[face->i[1]-1] += faceNormal;
			m_normals[face->i[2]-1] += faceNormal;
			std::copy(face->i, face->i+3, face->in);
		}
		// normalize generated normals
		for (std::vector<Vector3>::iterator normal = m_normals.begin(); normal != m_normals.end(); ++normal)
		{
			// only normalize normals that have been set (i.e. unused vertices will still have 0-length normals)
			if (normal->lengthSquared() > 0)
				normal->normalize();
		}
	}
#endif

#if 0
	// turn on textures if used
	m_textures = textureRange.min > 0;
#endif

	for ( size_t i = 0; i < _vertices.size(); ++i )
	{
		_vertices[i].x -= xrange.center();
		_vertices[i].x /= xrange.difference();
		_vertices[i].y -= yrange.center();
		_vertices[i].y /= yrange.difference();
		_vertices[i].z -= zrange.center();
		_vertices[i].z /= zrange.difference();
	}

	for ( size_t i = 0; i < _faces.size(); ++i )
	{
		for ( size_t j = 0; j < 3; ++j )
		{
			_faces[i].vertices.ix[j] -= 1;
			_faces[i].normals.ix[j] -= 1;
		}
	}

	using std::swap;
	swap( vertices, _vertices );
	swap( normals, _normals );
	swap( faces, _faces );
}
