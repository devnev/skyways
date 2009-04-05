
#include <vector>
#include "element.hpp"

class World
{
public:

	void glDraw()
	{
		for (ElementList::iterator elem = elements.begin();
				elem != elements.end(); ++elem)
		{
			elem->glDraw();
		}
	}

	void add(const Element& e)
	{
		elements.push_back(e);
	}

private:

	typedef std::vector<Element> ElementList;
	ElementList elements;
};
