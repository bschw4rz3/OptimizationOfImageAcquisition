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

#include "RasterService.h"

double RasterService::rasterResolution = 0.5;

RasterService::RasterService()
{

}

RasterResult RasterService::calculateBoarderRaster(double scale, AGeometry* facet, bool tritacticRaster)
{
	std::vector<Point> surefacePointList = facet->getContour(scale);

	// Berrechnen des Rasters
	RasterResult rasterResult;

	if (tritacticRaster)
	{
		rasterResult = this->calculateTritacticRaster(scale, facet);
	}
	else
	{
		rasterResult = this->calculateQuadraticRaster(scale, facet);
	}
	
	// Punkte zusammenführen
	std::vector<Point> surefaceRasterPoints = rasterResult.getSurefaceRasterPoints();

	for (int i = 0; i < surefaceRasterPoints.size(); i++)
	{
		Point surefaceRasterPoint = surefaceRasterPoints[i];
		surefacePointList.push_back(surefaceRasterPoint);
	}

	surefacePointList = this->toUniquePointList(surefacePointList);

	// Ergebniss zurückgeben
	return RasterResult(surefacePointList, rasterResult.getBackgroundRasterPoints());
}

void RasterService::drawLine(std::vector<Point>* list, Point from, Point end)
{
	Point start;
	Point finish;

	if (from.x < end.x)
	{
		start = from;
		finish = end;
	}
	else
	{
		start = end;
		finish = from;
	}

	double step = 0.5;
	double distanceX = (start.x - finish.x);
	double distanceY = (start.y - finish.y);

	double pointCount = distanceX / step;
	double times = 0;

	for (double x = start.x; x < finish.x; x+= step)
	{
		double y = start.y + (distanceY / pointCount * times);
		list->push_back(Point(x, y));

		times += 1;
	}

	list->push_back(end);
}

RasterResult RasterService::calculateQuadraticRaster(double scale, AGeometry* facet)
{
	RasterResult result;
	Point current(-1, -1);
	
	int begin = (int) facet->getMinY();
	int end = (int) facet->getMaxY();

	for (int x = begin; x <= end; x++)
	{
		this->drawRow(&result, scale, x, facet, begin, end, 1);
	}

	return result;
}

RasterResult RasterService::calculateTritacticRaster(double scale, AGeometry* facet)
{
	RasterResult result;

	double end = facet->getMinX();
	double begin = facet->getMaxX();

	double length = begin - end;
	double halfLength = (length / 2.0) + 2.5;

	double beginFull = -halfLength;
	double endFull = halfLength;

	double beginHalf = -(halfLength + 0.25);
	double endHalf = halfLength + 0.25;

	bool dings = false;

	double startX = facet->getMinY() - 1;
	double endX = facet->getMaxY() + 1;

	for (double x = startX; x <= endX; x+=rasterResolution)
	{
		if (dings)
		{
			this->drawRow(&result, scale, x, facet, beginHalf, endHalf, rasterResolution);
		}
		else
		{
			this->drawRow(&result, scale, x, facet, beginFull, endHalf, rasterResolution);
		}

		dings = !dings;
	}

	return result;
}

void RasterService::drawRow(RasterResult* result, double scale, double x, AGeometry* facet, double begin, double end, double raise)
{
	Point current(-1, -1);

	for (double y = begin; y <= end; y+= raise)
	{
		current.y = (x * scale);
		current.x = (y * scale);

		bool isOnSurface = facet->isOnSurface(Point(x * scale, y * scale));

		if (isOnSurface)
		{
			result->addSurefacePoint(current);
		}
		else
		{
			result->addBackgroundPoint(current);
		}
	}
}

std::vector<Point> RasterService::toUniquePointList(std::vector<Point> list)
{
	std::vector<Point> uniqueList;

	for (int i = 0; i < list.size(); i++)
	{
		if (std::find(uniqueList.begin(), uniqueList.end(), list[i]) == uniqueList.end())
		{
			// someName not in name, add it
			uniqueList.push_back(list[i]);
		}
	}

	return uniqueList;
}