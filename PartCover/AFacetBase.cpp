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

#include "AFacetBase.h"

std::vector<Point> AFacetBase::getContour(double scale)
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

std::vector<Point> AFacetBase::getContourMainPoints(double scale)
{
	return this->getContour(scale);
}

double AFacetBase::getMaxY()
{
	return max(this->getBegin(true), this->getBegin(false));
}

double AFacetBase::getMinY()
{
	return min(this->getEnd(true), this->getEnd(false));
}

Point AFacetBase::calculateBoarderPositive(std::vector<Point>* result, double scale)
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

Point AFacetBase::calculateBoarderNegative(std::vector<Point>* result, double scale)
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