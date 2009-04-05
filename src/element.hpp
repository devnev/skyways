
#include "block.hpp"

class Element
{
public:

	Element(double x, double y, double z, double l)
		: xoff(x), yoff(y), zoff(z), length(l)
	{
	}

	void glDraw()
	{
		glPushMatrix();
		glTranslated(xoff, yoff, zoff);
		glScaled(1, 1, length);
		block.glDraw();
		glPopMatrix();
	}

private:

	Block block;
	double xoff, yoff, zoff, length;

};
