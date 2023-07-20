#include "AObjectBase.h"

std::vector<Point> AObjectBase::getContour(double scale)
{
	std::vector<Point> result;

	Point next(-1, -1);
	Point firstPoint(this->calculateBoarder(this->getEnd(true), true) * scale, this->getEnd(true) * scale);
	Point current(-1, -1);
		
	current = this->calculateBoarderPositive(&result, scale);
	
	next.y = this->getBegin(false) * scale;
	next.x = this->calculateBoarder(this->getBegin(false), false) * scale;

	this->generateLine(&result, next, current);
	
	current = this->calculateBoarderNegative(&result, scale);

	this->generateLine(&result, current, firstPoint);

	return result;
}

double AObjectBase::getMaxY()
{
	return max(this->getBegin(true), this->getBegin(false));
}

double AObjectBase::getMinY()
{
	return min(this->getEnd(true), this->getEnd(false));
}

Point AObjectBase::calculateBoarderPositive(std::vector<Point>* result, double scale)
{
	Point current(-1, -1);

	for (double x = this->getEnd(true); x <= this->getBegin(true); x+=0.5)
	{
		current.y = x * scale;
		current.x = this->calculateBoarder(x, true) * scale;
		
		result->push_back(current);
	}

	return current;
}

Point AObjectBase::calculateBoarderNegative(std::vector<Point>* result, double scale)
{
	Point current(-1, -1);

	for (double x = this->getBegin(false); x >= this->getEnd(false); x-=0.5)
	{
		current.y = x * scale;
		current.x = this->calculateBoarder(x, false) * scale;

		result->push_back(current);
	}

	return current;
}