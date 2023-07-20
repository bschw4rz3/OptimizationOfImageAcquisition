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

#include "BananaPrimitive.h"

BananaPrimitive::BananaPrimitive(RasterService* rasterService, Picture* picture)
{
	this->position = new Point2D(0, 0);
	this->scale = 1;
	this->rasterScale = 1;
	
	this->rasterService = rasterService;
	this->picture = picture;

	this->facetColor = Color(255, 255, 0, 0);
	this->rasterColor = Color(200, 255, 255, 255);
	this->rasterOnSurfaceColor = Color(255, 0, 0, 255);
	this->solutionColor = Color(255, 0, 0, 255);
}

BananaPrimitive::~BananaPrimitive()
{
	delete this->position;
}

void BananaPrimitive::render(IPrimitiveGraphic* graphicEngine, IGeometry* facet)
{
	this->drawFacet(graphicEngine, facet);
	this->drawRaster(graphicEngine, facet);
	this->drawSolution(graphicEngine);
}

void BananaPrimitive::setOptimalSolution(std::vector<Point> solution, Point minPoint, std::vector<Point> corners)
{
	this->minPoint = minPoint;
	this->corners = corners;
	this->solution = solution;
}

void BananaPrimitive::drawFacet(IPrimitiveGraphic* graphicEngine, IGeometry* facet)
{
	Point2D firstPoint(-1, -1);
	Point2D lastPoint(-1, -1);
	Point2D current(-1, -1);

	std::vector<Point> contour = ((AGeometry*) facet)->getContour(scale);

	firstPoint.x = this->position->x + contour[0].x;
	firstPoint.y = this->position->y + contour[0].y;
	lastPoint = firstPoint;

	for(int i = 0; i < contour.size() ; i++)
	{
		current.x = this->position->x + contour[i].x;
		current.y = this->position->y + contour[i].y;

		graphicEngine->add2DLine(lastPoint, current, this->facetColor);

		lastPoint = current;
	}

	if(this->minPoint.x < 1000)
	{
		Color color(255,0,255, 0);
		Color cornerColor(255,0,0, 255);

		this->drawFatPoint(graphicEngine, this->minPoint, color);

		for(int j = 0;j < this->corners.size(); j++)
		{
			this->drawFatPoint(graphicEngine, this->corners[j], cornerColor);
		}
	}
}

void BananaPrimitive::drawFatPoint(IPrimitiveGraphic* graphicEngine, Point minPoint, Color color)
{
	Point2D minTopPoint = Point2D(this->position->x + (minPoint.x * scale), this->position->y + (minPoint.y * scale));
		
	graphicEngine->add2DPixel(minTopPoint, color);
	graphicEngine->add2DPixel(Point2D(minTopPoint.x, minTopPoint.y+1), color);
	graphicEngine->add2DPixel(Point2D(minTopPoint.x+1, minTopPoint.y), color);
	graphicEngine->add2DPixel(Point2D(minTopPoint.x, minTopPoint.y-1), color);
	graphicEngine->add2DPixel(Point2D(minTopPoint.x-1, minTopPoint.y), color);
}

void BananaPrimitive::drawRaster(IPrimitiveGraphic* graphicEngine, IGeometry* facet)
{
	RasterResult rasterResult;

	Color currentColor;
	Point2D current(-1, -1);

	if (RASTER_TRITACTIC)
	{
		rasterResult = this->rasterService->calculateTritacticRaster(this->rasterScale, (AGeometry*)facet);
	}
	else
	{
		rasterResult = this->rasterService->calculateQuadraticRaster(this->rasterScale, (AGeometry*)facet);
	}

	this->drawRaster(graphicEngine, rasterResult.getBackgroundRasterPoints(), this->rasterColor);
	this->drawRaster(graphicEngine, rasterResult.getSurefaceRasterPoints(), this->facetColor);
}

void BananaPrimitive::drawRaster(IPrimitiveGraphic* graphicEngine, const vector<Point> rasterPointList, Color color)
{
	Point2D current(-1, -1);

	for (int i = 0; i < rasterPointList.size(); i++)
	{
		current.x = this->position->x + (rasterPointList[i].x * this->scale);
		current.y = this->position->y + (rasterPointList[i].y * this->scale);

		graphicEngine->add2DPixel(current, color);
	}
}

void BananaPrimitive::drawSolution(IPrimitiveGraphic* graphicEngine)
{
	Point2D current(-1, -1);

	double totalScale = this->rasterScale * this->scale;

	double halfWidth = (this->picture->width / 2 * totalScale);
	double halfHeigth = (this->picture->height / 2 * totalScale);

	for (int i = 0; i < this->solution.size(); i++)
	{
		current.x = this->position->x + (this->solution[i].x * totalScale);
		current.y = this->position->y + (this->solution[i].y * totalScale);

		graphicEngine->add2DPixel(current, this->solutionColor);
		graphicEngine->add2DRectangle(Point2D(current.x - halfWidth, current.y - halfHeigth), Point2D(current.x + halfWidth, current.y + halfHeigth), this->solutionColor);
	}
}