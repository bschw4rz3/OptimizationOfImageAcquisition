#include "RotatedHexagonObject.h"

RotatedHexagonObject::RotatedHexagonObject(double width, double height)
{
	this->width = width;
	this->height = height;
}

bool RotatedHexagonObject::isOnSurface(Point point)
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

std::vector<Point> RotatedHexagonObject::getContour(double scale)
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
	
double RotatedHexagonObject::getMaxY() 
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

double RotatedHexagonObject::getMinY() 
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


double RotatedHexagonObject::getHalfWidth(double scale)
{
	return 0.5*this->width*scale;
}

double RotatedHexagonObject::getHalfHeight(double scale)
{
	return 0.5*this->height*scale;
}

double RotatedHexagonObject::getFourParter(double scale)
{
	double halfWidth = this->getHalfWidth(scale);
	return 0.65*halfWidth;	
}

bool RotatedHexagonObject::isInLeftArrow(Point point)
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

bool RotatedHexagonObject::isInRightArrow(Point point)
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