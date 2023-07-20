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

#include "PointedPupilFacet.h"

PointedPupilFacet::PointedPupilFacet(double width, double height)
{
	this->width = width;
	this->height = height;
}

bool PointedPupilFacet::isOnSurface(Point point)
{
	double halfWidth = this->getHalfWidth(1.0);
	double halfHeight = this->getHalfHeight(1.0);
	double fourParter = this->getFourParter(1.0);

	if(point.y >= -fourParter && point.y <= fourParter && 
	   point.x >= -halfHeight && point.x <= halfHeight)
	{
		if(point.x < 0 && point.y > 0)
		{
			double m = this->calculateLinearM(Point(0, -halfHeight), Point(fourParter, -halfHeight/2));
			double b = this->calculateLinearB(Point(0, -halfHeight), m);	

			double topY = m * point.y + b;
			return topY < point.x;
		}
		else if(point.x >= 0 && point.y >= 0)
		{
			double m = this->calculateLinearM(Point(0, halfHeight), Point(fourParter, halfHeight/2));
			double b = this->calculateLinearB(Point(0, halfHeight), m);	

			double topY = m * point.y + b;
			return topY > point.x;
		}
		else if(point.x < 0 && point.y < 0)
		{
			double m = this->calculateLinearM(Point(-fourParter, -halfHeight/2), Point(0, -halfHeight));
			double b = this->calculateLinearB(Point(-fourParter, -halfHeight/2), m);	

			double topY = m * point.y + b;
			return topY < point.x;
		}
		else if(point.x >= 0 && point.y <= 0)
		{
			double m = this->calculateLinearM(Point(-fourParter, halfHeight/2), Point(0, halfHeight));
			double b = this->calculateLinearB(Point(-fourParter, halfHeight/2), m);	

			double topY = m * point.y + b;
			return topY > point.x;
		}

		return true;
	}

	return false;
}

std::vector<Point> PointedPupilFacet::getContour(double scale)
{
	std::vector<Point> geomeriePoints;

	double halfWidth = this->getHalfWidth(scale);
	double halfHeight = this->getHalfHeight(scale);
	double fourParter = this->getFourParter(scale);

	geomeriePoints.push_back(Point(0, halfHeight));
	geomeriePoints.push_back(Point(fourParter, halfHeight/2));
	geomeriePoints.push_back(Point(fourParter, -halfHeight/2));

	geomeriePoints.push_back(Point(0, -halfHeight));
	geomeriePoints.push_back(Point(-fourParter, -halfHeight/2));
	geomeriePoints.push_back(Point(-fourParter, halfHeight/2));

	geomeriePoints.push_back(Point(0, halfHeight));

	std::vector<Point> conture;

	for(int i = 1; i < geomeriePoints.size() ;i++)
	{
		this->generateLine(&conture, geomeriePoints[i], geomeriePoints[i-1]);
	}

	return conture;
}

std::vector<Point> PointedPupilFacet::getContourMainPoints(double scale)
{
	return this->getContour(scale);
}

double PointedPupilFacet::getHalfWidth(double scale)
{
	return 0.5*this->width*scale;
}

double PointedPupilFacet::getHalfHeight(double scale)
{
	return 0.5*this->height*scale;
}

double PointedPupilFacet::getFourParter(double scale)
{
	double halfWidth = this->getHalfWidth(scale);
	return 0.65*halfWidth;	
}

bool PointedPupilFacet::isInLeftArrow(Point point)
{
	double scale = 1.0;

	double halfWidth = this->getHalfWidth(scale);
	double halfHeight = this->getHalfHeight(scale);
	double fourParter = this->getFourParter(scale);
	
	Point start = Point(-fourParter, -halfHeight);
	Point middle = Point(-halfWidth, 0);
	Point end = Point(-fourParter, halfHeight);

	if(start.x > point.y && middle.x < point.y)
	{
		if(point.x < 0)
		{
			double m = this->calculateLinearM(start, middle);
			double b = this->calculateLinearB(start, m);	

			double topY = m * point.y + b;
			return topY < point.x;
		}
		else
		{
			double m = this->calculateLinearM(middle, end);
			double b = this->calculateLinearB(end, m);	

			double topY = m * point.y + b;
			return topY > point.x;
		}
	}

	return false;
}

bool PointedPupilFacet::isInRightArrow(Point point)
{
	double scale = 1.0;

	double halfWidth = this->getHalfWidth(scale);
	double halfHeight = this->getHalfHeight(scale);
	double fourParter = this->getFourParter(scale);
	
	Point start = Point(fourParter, halfHeight);
	Point middle = Point(halfWidth, 0);
	Point end = Point(fourParter, -halfHeight);

	if(start.x < point.y && middle.x > point.y)
	{
		if(point.x > 0)
		{
			double m = this->calculateLinearM(start, middle);
			double b = this->calculateLinearB(start, m);	

			double topY = m * point.y + b;
			return topY > point.x;
		}
		else
		{
			double m = this->calculateLinearM(middle, end);
			double b = this->calculateLinearB(end, m);	

			double topY = m * point.y + b;
			return topY < point.x;
		}
	}

	return false;
}