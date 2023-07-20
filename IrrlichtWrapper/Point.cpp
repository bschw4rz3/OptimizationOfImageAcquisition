#include "Point.h"

Point::Point()
{

}

Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

bool Point::operator== (Point& a)
{
	return a.x == this->x && a.y == this->y;
}

bool Point::operator== (Point* a)
{
	return a->x == this->x && a->y == this->y;
}

bool Point::operator== (const Point& a)
{
	return a.x == this->x && a.y == this->y;
}

bool Point::operator== (const Point* a)
{
	return a->x == this->x && a->y == this->y;
}

bool Point::operator< (const Point& a) const
{
	if(this->x < a.x)
	{
		return true;
	}
	else if(this->x == a.x)
	{
		return this->y < a.y;	
	}
	
    return false;
}