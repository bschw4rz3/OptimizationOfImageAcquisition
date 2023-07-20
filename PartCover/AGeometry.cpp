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

#include "AGeometry.h"

double AGeometry::contourResolution = 0.25;

void AGeometry::generateLine(std::vector<Point>* result, Point next, Point start)
{
	double distanceX = next.x - start.x;
	double distanceY = next.y - start.y;

	Point current;
	double iterationen = distanceX != 0 ? abs(distanceX) : abs(distanceY);
	iterationen *= 4;

	for(double i = 0 ; i < (int) iterationen ; i+=contourResolution)
	{
		current.x = start.x + distanceX / iterationen * (i);
		current.y = start.y + distanceY / iterationen * (i);

		result->push_back(current);
	}
}

double AGeometry::calculateLinearM(Point facetBegin1, Point facetBegin2)
{
	return (facetBegin2.y - facetBegin1.y) / (facetBegin2.x - facetBegin1.x);
}

double AGeometry::calculateLinearB(Point facetBegin1, double m)
{
	return facetBegin1.y - m * facetBegin1.x;
}

double AGeometry::getMaxX() 
{
	double x = FLT_MIN;
	std::vector<Point> contour = this->getContour(1);

	for(int i = 0; i < contour.size() ; i++)
	{
		if(x < contour[i].x)
		{
			x = contour[i].x;
		}
	}

	return x;
}

double AGeometry::getMinX() 
{
	double x = FLT_MAX;
	std::vector<Point> contour = this->getContour(1);

	for(int i = 0; i < contour.size() ; i++)
	{
		if(x > contour[i].x)
		{
			x = contour[i].x;
		}
	}

	return x;
}
	
double AGeometry::getMaxY() 
{
	double y = FLT_MIN;
	std::vector<Point> contour = this->getContour(1);

	for(int i = 0; i < contour.size() ; i++)
	{
		if(y < contour[i].y)
		{
			y = contour[i].y;
		}
	}

	return y;
}

double AGeometry::getMinY() 
{
	double y = FLT_MAX;
	std::vector<Point> contour = this->getContour(1);

	for(int i = 0; i < contour.size() ; i++)
	{
		if(y > contour[i].y)
		{
			y = contour[i].y;
		}
	}

	return y;
}

double AGeometry::getArea()
{
	double area = 0;
	std::vector<Point> contour = this->getContourMainPoints(1.0);

	for (int i = 0; i < contour.size();i++)
	{
		if(i == 0)
		{
			continue;
		}

		Point lastPoint = contour[i-1];

		//if(i == contour.size()-1)
		//{
			//lastPoint = contour[0];
		//}

		Point currentPoint = contour[i];

		area += (lastPoint.y + currentPoint.y) * (lastPoint.x - currentPoint.x);
	}

	return 0.5*area;
}