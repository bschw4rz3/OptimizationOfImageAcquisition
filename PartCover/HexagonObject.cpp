#include "HexagonObject.h"

HexagonObject::HexagonObject(double width, double height)
{
	this->width = width;
	this->height = height;
}

bool HexagonObject::isOnSurface(Point point)
{
	double halfHeight = this->getHalfHeight(1.0);
	double fourParter = this->getFourParter(1.0);

	if(point.x <= halfHeight && point.x >= -halfHeight)
	{
		if(point.y <= fourParter && point.y >= -fourParter)
		{
			return true;
		}	
	}

	return this->isInLeftArrow(point) || this->isInRightArrow(point);
}

std::vector<Point> HexagonObject::getContour(double scale)
{
	std::vector<Point> geomeriePoints;

	double halfWidth = this->getHalfWidth(scale);
	double halfHeight = this->getHalfHeight(scale);
	double fourParter = this->getFourParter(scale);

	geomeriePoints.push_back(Point(-halfWidth, 0));
	geomeriePoints.push_back(Point(-fourParter, halfHeight));
	geomeriePoints.push_back(Point(fourParter, halfHeight));

	geomeriePoints.push_back(Point(halfWidth, 0));
	geomeriePoints.push_back(Point(fourParter, -halfHeight));
	geomeriePoints.push_back(Point(-fourParter, -halfHeight));

	geomeriePoints.push_back(Point(-halfWidth, 0));

	std::vector<Point> conture;

	for(int i = 1; i < geomeriePoints.size() ;i++)
	{
		this->generateLine(&conture, geomeriePoints[i], geomeriePoints[i-1]);
	}

	return conture;
}
	
double HexagonObject::getMaxY() 
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

double HexagonObject::getMinY() 
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


double HexagonObject::getHalfWidth(double scale)
{
	return 0.5*this->width*scale;
}

double HexagonObject::getHalfHeight(double scale)
{
	return 0.5*this->height*scale;
}

double HexagonObject::getFourParter(double scale)
{
	double halfWidth = this->getHalfWidth(scale);
	return 0.65*halfWidth;	
}

bool HexagonObject::isInLeftArrow(Point point)
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

bool HexagonObject::isInRightArrow(Point point)
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