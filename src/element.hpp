
#include "block.hpp"

class Element
{
public:

	Element(double x, double y, double z)
		: xoff(x), yoff(y), zoff(z)
	{
	}

	void glDraw()
	{
		glTranslated(xoff, yoff, zoff);
		block.glDraw();
		glTranslated(-xoff, -yoff, -zoff);
	}

private:

	Block block;
	double xoff, yoff, zoff;

};
