#pragma once

class Point {
public:
	double x;
	double y;

public:
	Point();
	Point(double x, double y);

	bool operator== (Point& a);
	bool operator== (Point* a);
	bool operator== (const Point& a);
	bool operator== (const Point* a);
	
	bool operator<(const Point& a) const;
};