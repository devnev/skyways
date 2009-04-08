#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <memory>
#include <vector>
#include <istream>
#include "vector.hpp"

class Block
{

public:

	void glDraw();

	static std::auto_ptr< Block > fromStream( std::istream& is );

private:

	static void getline( std::istream& is, std::string& line );

	typedef struct { Vector3 points[4]; Vector3& operator[](size_t index) { return points[index]; } } Face;
	typedef std::vector< Face > FaceList;
	FaceList faces;

};

#endif // _BLOCK_HPP_
