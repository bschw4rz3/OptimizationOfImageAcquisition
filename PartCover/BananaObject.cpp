#include "BananaObject.h"

BananaObject::BananaObject(double length, double width, double radius)
{
	this->length = length;
	this->width = width;
	this->radius = radius;
}

vector<Point> BananaObject::getRaster(double detailFactor)
{
	vector<Point> list;

	return list;
}

double BananaObject::calculateBoarder(double x, bool positiv)
{
	double endOfFacet = this->length / 2;

	if (endOfFacet < x || -endOfFacet > x)
	{
		return NAN;
	}

	double direction = positiv ? 1 : -1;

	double powResult = pow(this->radius, 2) - pow(x, 2);
	double sqrtResult = sqrt(powResult);

	return sqrtResult - this->radius + (direction * this->width/2);
}

bool BananaObject::isOnSurface(Point point)
{
	double halfLength = this->length / 2;

	if (point.x >= halfLength || point.x <= -halfLength)
	{
		return false;
	}

	double negativeY = this->calculateBoarder(point.x, false);
	double positiveY = this->calculateBoarder(point.x, true);

	if (isnan(negativeY) || isnan(positiveY))
	{
		return false;
	}

	return negativeY <= point.y && point.y <= positiveY;
}

double BananaObject::getBegin(bool positiv)
{
	return this->length / 2;
}

double BananaObject::getEnd(bool positiv)
{
	return -(this->length / 2);
}

double BananaObject::getLength(bool positiv)
{
	return this->length;
}