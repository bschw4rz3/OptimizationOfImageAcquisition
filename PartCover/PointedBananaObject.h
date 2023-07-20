#ifndef PointedBananaFacet_H
#define PointedBananaFacet_H

#include <vector>

#include "AObjectBase.h"
#include "Point.h"

using namespace std;

class PointedBananaObject : AObjectBase
{
private:
	double length;
	double width;

	double topDiff;
	double radius;

	bool positivSector;

public:
	PointedBananaObject(double length, double width, double radius, double topDiff, bool direction = true);

	vector<Point> getRaster(double detailFactor);
	double calculateBoarder(double x, bool positiv);

	bool isOnSurface(Point point);

	double getBegin(bool positiv);
	double getEnd(bool positiv);
	double getLength(bool positiv);

private:
	Point calculateBegin(bool positiv);
	Point calculateEnd(bool positiv);

	double calculateBeginY(double pointX);
	double calculateEndY(double pointX);
};

#endif