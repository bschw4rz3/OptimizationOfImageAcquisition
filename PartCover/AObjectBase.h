#ifndef AFacetBase_H
#define AFacetBase_H

#include <vector>

#include "AGeometry.h"
#include "Point.h"

#include "../IrrlichtWrapper/Point2D.h"
#include "../IrrlichtWrapper/Color.h"
#include "../IrrlichtWrapper/IPrimitiveGraphic.h"

using namespace std;

class AObjectBase : public AGeometry
{
public:

	virtual vector<Point> getRaster(double detailFactor) = 0;
	virtual double calculateBoarder(double x, bool positiv) = 0;

	virtual bool isOnSurface(Point point) = 0;

	virtual double getBegin(bool positiv) = 0;
	virtual double getEnd(bool positiv) = 0;
	virtual double getLength(bool positiv) = 0;
	
public:
	std::vector<Point> getContour(double scale);

	double getMaxY();
	double getMinY();

private:
	Point calculateBoarderPositive(std::vector<Point>* result, double scale);
	Point calculateBoarderNegative(std::vector<Point>* result, double scale);
};

#endif