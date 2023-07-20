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

#include "Lens.h"

Lens::Lens(double length, double width, double radius, double topDiff, bool positivSector)
{
	this->length = length;
	this->width = width;
	this->radius = radius;
	this->topDiff = topDiff;
	this->positivSector = positivSector;
}

vector<Point> Lens::getRaster(double detailFactor)
{
	vector<Point> list;

	return list;
}

std::vector<Point> Lens::getContour(double scale)
{
	std::vector<Point> result;
			
	this->calculateBoarderPositive(&result, scale);
	
	this->calculateBoarderNegative(&result, scale);

	result.push_back(this->getLineBegin(scale));

	return result;
}

std::vector<Point> Lens::getContourMainPoints(double scale)
{
	return this->getContour(scale);
}


Point Lens::calculateBoarderPositive(std::vector<Point>* result, double scale)
{
	Point current(-1, -1);

	this->positivSector = true;

	for (double x = this->getEnd(true); x <= this->getBegin(true); x+=0.5)
	{
		current.y = x * scale;
		current.x = this->calculateBoarder(x, true) * scale;

		if(!isnan(current.x))
		{
			result->push_back(current);
		}
	}

	return current;
}

Point Lens::getLineBegin(double scale)
{
	Point current;

	this->positivSector = true;
	double x = this->getEnd(false);

	current.y = x * scale;
	current.x = this->calculateBoarder(x, true) * scale;

	return current;
}

Point Lens::calculateBoarderNegative(std::vector<Point>* result, double scale)
{
	Point current(-1, -1);

	this->positivSector = false;

	for (double x = this->getBegin(false); x >= this->getEnd(false); x-=0.5)
	{
		current.y = x * scale;
		current.x = this->calculateBoarder(x, false) * scale;

		if(!isnan(current.x))
		{
			result->push_back(current);
		}
	}

	return current;
}

double Lens::calculateBoarder(double x, bool positiv)
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
	double move = this->positivSector ? 4.5 : 8;

	double powResult = pow(this->radius, 2) - pow(x, 2);
	double sqrtResult = sector * sqrt(powResult) + move;

	return sqrtResult - this->radius + (direction * this->width/2);
}

bool Lens::isOnSurface(Point point)
{
	double halfLength = this->length / 2;

	if (point.x >= halfLength || point.x <= -halfLength)
	{
		return false;
	}

	this->positivSector = false;
	double negativeY = this->calculateBoarder(point.x, false);

	this->positivSector = true;
	double positiveY = this->calculateBoarder(point.x, true);

	if (isnan(negativeY) || isnan(positiveY))
	{
		return false;
	}

	return negativeY <= point.y && point.y <= positiveY;
}

double Lens::getBegin(bool positiv)
{
	double facetEnd = this->length / 2;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return facetEnd - 8.5;
}

double Lens::getEnd(bool positiv)
{
	double facetEnd = this->length / 2;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return -facetEnd + 8.5;
}

double Lens::getLength(bool positiv)
{
	double facetEnd = this->length;

	if((positiv && this->positivSector) || !positiv && !this->positivSector)
	{
		facetEnd -= this->topDiff;
	}

	return -facetEnd;
}

Point Lens::calculateBegin(bool positiv)
{
	double facetBeginX = this->getBegin(positiv);
	double facetBeginY = this->calculateBoarder(facetBeginX, positiv);

	return Point(facetBeginX, facetBeginY);
}

Point Lens::calculateEnd(bool positiv)
{
	double facetBeginX = this->getEnd(positiv);
	double facetBeginY = this->calculateBoarder(facetBeginX, positiv);

	return Point(facetBeginX, facetBeginY);
}

double Lens::calculateBeginY(double pointX)
{
	Point facetBegin1 = this->calculateBegin(true);
	Point facetBegin2 = this->calculateBegin(false);
	
	double m = this->calculateLinearM(facetBegin1, facetBegin2);
	double b = this->calculateLinearB(facetBegin1, m);

	return m * pointX + b;
}

double Lens::calculateEndY(double pointX)
{
	Point facetEnd1 = this->calculateEnd(true);
	Point facetEnd2 = this->calculateEnd(false);
	
	double m = this->calculateLinearM(facetEnd1, facetEnd2);
	double b = this->calculateLinearB(facetEnd1, m);

	return m * pointX + b;
}