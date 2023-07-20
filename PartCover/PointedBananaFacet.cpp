// BSD 3 - Copyright 2023
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software 
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
// DAMAGE.
// 
// Parts of this Software are based on Irrlicht Engine, zlib, libpng and IJG-Code

#include "PointedBananaFacet.h"

PointedBananaFacet::PointedBananaFacet(double length, double width, double radius, double topDiff, bool positivSector)
{
	this->length = length;
	this->width = width;
	this->radius = radius;
	this->topDiff = topDiff;
	this->positivSector = positivSector;
}

vector<Point> PointedBananaFacet::getRaster(double detailFactor)
{
	vector<Point> list;

	return list;
}

double PointedBananaFacet::calculateBoarder(double x, bool positiv)
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

bool PointedBananaFacet::isOnSurface(Point point)
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

double PointedBananaFacet::getBegin(bool positiv)
{
	double facetEnd = this->length / 2;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return facetEnd;
}

double PointedBananaFacet::getEnd(bool positiv)
{
	double facetEnd = this->length / 2;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return -facetEnd;
}

double PointedBananaFacet::getLength(bool positiv)
{
	double facetEnd = this->length;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return -facetEnd;
}

Point PointedBananaFacet::calculateBegin(bool positiv)
{
	double facetBeginX = this->getBegin(positiv);
	double facetBeginY = this->calculateBoarder(facetBeginX, positiv);

	return Point(facetBeginX, facetBeginY);
}

Point PointedBananaFacet::calculateEnd(bool positiv)
{
	double facetBeginX = this->getEnd(positiv);
	double facetBeginY = this->calculateBoarder(facetBeginX, positiv);

	return Point(facetBeginX, facetBeginY);
}

double PointedBananaFacet::calculateBeginY(double pointX)
{
	Point facetBegin1 = this->calculateBegin(true);
	Point facetBegin2 = this->calculateBegin(false);
	
	double m = this->calculateLinearM(facetBegin1, facetBegin2);
	double b = this->calculateLinearB(facetBegin1, m);

	return m * pointX + b;
}

double PointedBananaFacet::calculateEndY(double pointX)
{
	Point facetEnd1 = this->calculateEnd(true);
	Point facetEnd2 = this->calculateEnd(false);
	
	double m = this->calculateLinearM(facetEnd1, facetEnd2);
	double b = this->calculateLinearB(facetEnd1, m);

	return m * pointX + b;
}