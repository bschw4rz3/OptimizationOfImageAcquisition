#ifndef PupilFacet_h
#define PupilFacet_h

#include <vector>
#include "AGeometry.h"

class HexagonObject : AGeometry
{
private:
	double width;
	double height;

public:

	HexagonObject(double width, double height);

	bool isOnSurface(Point point);
	std::vector<Point> getContour(double scale);
	
	double getMaxY();
	double getMinY();

private:
	double getHalfWidth(double scale);
	double getHalfHeight(double scale);
	double getFourParter(double scale);

	bool isInLeftArrow(Point point);
	bool isInRightArrow(Point point);
};

#endif