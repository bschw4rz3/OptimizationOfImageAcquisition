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

#include "CarDoor.h"

CarDoor::CarDoor(double width, double height)
{
	this->width = width;
	this->height = height;
}

bool CarDoor::isOnSurface(Point point)
{
	double halfWidth = this->getHalfWidth(1.0);
	double halfHeight = this->getHalfHeight(1.0);

	Point rightStart = Point(0, halfHeight);
	Point leftStart = Point(halfWidth/2, halfHeight);
	Point right = Point(halfWidth, -halfHeight);
	Point left = Point(-halfWidth*0.8, -halfHeight);
	Point newPoint = Point(-halfWidth, 0);

	if(halfHeight < point.x || -halfHeight > point.x)
	{
		return false;
	}

	double m = this->calculateLinearM(leftStart, right);
	double b = this->calculateLinearB(leftStart, m);	

	double leftY = m * point.y + b;

	if(leftY > point.x)
	{
		double m = this->calculateLinearM(newPoint, left);
		double b = this->calculateLinearB(newPoint, m);	

		double rightY = m * point.y + b;

		if(point.x > rightY)
		{
			double m = this->calculateLinearM(newPoint, rightStart);
			double b = this->calculateLinearB(newPoint, m);	

			double newPointY = m * point.y + b;

			return point.x < newPointY;
		}
	}

	return false;
}

std::vector<Point> CarDoor::getContour(double scale)
{
	std::vector<Point> geomeriePoints = this->getContourMainPoints(scale);

	std::vector<Point> conture;

	for(int i = 1; i < geomeriePoints.size() ;i++)
	{
		this->generateLine(&conture, geomeriePoints[i], geomeriePoints[i-1]);
	}

	return conture;
}

std::vector<Point> CarDoor::getContourMainPoints(double scale)
{
	std::vector<Point> geomeriePoints;

	double halfWidth = this->getHalfWidth(scale);
	double halfHeight = this->getHalfHeight(scale);

	geomeriePoints.push_back(Point(halfWidth/2, halfHeight));
	geomeriePoints.push_back(Point(halfWidth, -halfHeight));
	geomeriePoints.push_back(Point(-halfWidth*0.8, -halfHeight));
	geomeriePoints.push_back(Point(-halfWidth, 0));
	geomeriePoints.push_back(Point(0, halfHeight));
	geomeriePoints.push_back(Point(halfWidth/2, halfHeight));

	return geomeriePoints;
}

double CarDoor::getHalfWidth(double scale)
{
	return 0.5*this->height*scale;
}

double CarDoor::getHalfHeight(double scale)
{
	return 0.5*this->width*scale;
}