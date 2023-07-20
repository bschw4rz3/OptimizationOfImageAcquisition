#include "PointedBananaObject.h"

PointedBananaObject::PointedBananaObject(double length, double width, double radius, double topDiff, bool positivSector)
{
	this->length = length;
	this->width = width;
	this->radius = radius;
	this->topDiff = topDiff;
	this->positivSector = positivSector;
}

vector<Point> PointedBananaObject::getRaster(double detailFactor)
{
	vector<Point> list;

	return list;
}

double PointedBananaObject::calculateBoarder(double x, bool positiv)
{
	double endOfFacet = this->length / 2;
	
	if (endOfFacet < x || -endOfFacet > x)
	{
		return NAN;
	}

	if(this->positivSector)
	{
		if(positiv)
		{
			if (endOfFacet - this->topDiff < x || -(endOfFacet- this->topDiff) > x)
			{
				return NAN;
			}
		}
	}
	else
	{
		if(!positiv)
		{
			if (endOfFacet - this->topDiff < x || -(endOfFacet- this->topDiff) > x)
			{
				return NAN;
			}
		}
	}

	double direction = positiv ? 1 : -1;

	double sector = this->positivSector ? 1 : -1;
	double move = this->positivSector ? 0 : 25;

	double powResult = pow(this->radius, 2) - pow(x, 2);
	double sqrtResult = sector * sqrt(powResult) + move;

	return sqrtResult - this->radius + (direction * this->width/2);
}

bool PointedBananaObject::isOnSurface(Point point)
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
		if(point.x > 0)
		{
			double beginY = this->calculateBeginY(point.x);

			if((beginY >= point.y && this->positivSector) || (beginY <= point.y && !this->positivSector))
			{
				if(!isnan(negativeY))
				{
					return negativeY <= point.y;
				}
				else if(!isnan(positiveY))
				{
					return point.y <= positiveY;
				}
			}
		}
		else
		{
			double endY = this->calculateEndY(point.x);

			if((endY >= point.y && this->positivSector) || (endY <= point.y && !this->positivSector))
			{
				if(!isnan(negativeY))
				{
					return negativeY <= point.y;
				}
				else if(!isnan(positiveY))
				{
					return point.y <= positiveY;
				}
			}
		}

		return false;
	}
	
	return negativeY <= point.y && point.y <= positiveY;
}

double PointedBananaObject::getBegin(bool positiv)
{
	double facetEnd = this->length / 2;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return facetEnd;
}

double PointedBananaObject::getEnd(bool positiv)
{
	double facetEnd = this->length / 2;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return -facetEnd;
}

double PointedBananaObject::getLength(bool positiv)
{
	double facetEnd = this->length;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return -facetEnd;
}

Point PointedBananaObject::calculateBegin(bool positiv)
{
	double facetBeginX = this->getBegin(positiv);
	double facetBeginY = this->calculateBoarder(facetBeginX, positiv);

	return Point(facetBeginX, facetBeginY);
}

Point PointedBananaObject::calculateEnd(bool positiv)
{
	double facetBeginX = this->getEnd(positiv);
	double facetBeginY = this->calculateBoarder(facetBeginX, positiv);

	return Point(facetBeginX, facetBeginY);
}

double PointedBananaObject::calculateBeginY(double pointX)
{
	Point facetBegin1 = this->calculateBegin(true);
	Point facetBegin2 = this->calculateBegin(false);
	
	double m = this->calculateLinearM(facetBegin1, facetBegin2);
	double b = this->calculateLinearB(facetBegin1, m);

	return m * pointX + b;
}

double PointedBananaObject::calculateEndY(double pointX)
{
	Point facetEnd1 = this->calculateEnd(true);
	Point facetEnd2 = this->calculateEnd(false);
	
	double m = this->calculateLinearM(facetEnd1, facetEnd2);
	double b = this->calculateLinearB(facetEnd1, m);

	return m * pointX + b;
}