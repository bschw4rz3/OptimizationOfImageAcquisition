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

#include "BananaFacet.h"

BananaFacet::BananaFacet(double length, double width, double radius)
{
	this->length = length;
	this->width = width;
	this->radius = radius;
}

vector<Point> BananaFacet::getRaster(double detailFactor)
{
	vector<Point> list;

	return list;
}

double BananaFacet::calculateBoarder(double x, bool positiv)
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

bool BananaFacet::isOnSurface(Point point)
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

double BananaFacet::getBegin(bool positiv)
{
	return this->length / 2;
}

double BananaFacet::getEnd(bool positiv)
{
	return -(this->length / 2);
}

double BananaFacet::getLength(bool positiv)
{
	return this->length;
}