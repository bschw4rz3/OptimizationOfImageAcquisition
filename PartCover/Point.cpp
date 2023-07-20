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

#include "Point.h"

Point::Point()
{

}

Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

bool Point::operator== (IrrPoint& a)
{
	return a.getX() == this->x && a.getY() == this->y;
}

bool Point::operator== (IrrPoint* a)
{
	return a->getX() == this->x && a->getY() == this->y;
}

bool Point::operator== (const IrrPoint& a)
{
	return a.getX() == this->x && a.getY() == this->y;
}

bool Point::operator== (const IrrPoint* a)
{
	return a->getX() == this->x && a->getY() == this->y;
}

bool Point::operator< (const IrrPoint& a) const
{
	if(this->x < a.getX())
	{
		return true;
	}
	else if(this->x == a.getX())
	{
		return this->y < a.getY();	
	}
	
    return false;
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
	if (this->x < a.x)
	{
		return true;
	}
	else if (this->x == a.x)
	{
		return this->y < a.y;
	}

	return false;
}

double Point::getX() const
{
	return this->x;
}

double Point::getY() const
{
	return this->y;
}