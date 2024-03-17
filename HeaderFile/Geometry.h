#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

struct Geometry
{
	int GetNDet()
    {
        return zs.size();
    };
	std::vector<float> zs;
};

#endif
