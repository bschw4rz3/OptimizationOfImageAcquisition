#ifndef RectObject_H
#define RectObject_H

#include <vector>

#include "AObjectBase.h"
#include "Point.h"

using namespace std;

class RectObject : AObjectBase
{
private:
	double length;
	double width;

public:
	RectObject(double length, double width);

	vector<Point> getRaster(double detailFactor);
	double calculateBoarder(double x, bool positiv);

	bool isOnSurface(Point point);

	double getBegin(bool positiv);
	double getEnd(bool positiv);
	double getLength(bool positiv);
};

#endif