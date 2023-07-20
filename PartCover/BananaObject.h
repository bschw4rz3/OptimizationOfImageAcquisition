#ifndef BananaFacet_H
#define BananaFacet_H

#include <vector>

#include "AObjectBase.h"
#include "Point.h"

using namespace std;

class BananaObject : AObjectBase
{
private:
	double length;
	double width;

	double radius;

public:
	BananaObject(double length, double width, double radius);

	vector<Point> getRaster(double detailFactor);
	double calculateBoarder(double x, bool positiv);

	bool isOnSurface(Point point);

	double getBegin(bool positiv);
	double getEnd(bool positiv);
	double getLength(bool positiv);
};

#endif