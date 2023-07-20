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

#ifndef QuadSortService_H
#define QuadSortService_H

#include <sstream>
#include <iostream>
#include <bitset>
#include <map>

#include "../PartCover/RasterManager.h"
#include "../PartCover/MathHelper.h"
#include "../PartCover/Picture.h"
#include "CalculationQuadResult.h"
#include "CalculationHelper.h"

class QuadSortService 
{
public:
	static double angleDetectionRadius;
	static int minAngle;

private:
	RasterManager* rasterManager;
	MathHelper* mathHelper;
	CalculationHelper* calculationHelper;
	
	bool holeFillingMode;

	std::map<Point, std::vector<Point>> pointCache;

public:
	Point topLeftCorner;
	Point contourMarker;

	std::vector<Point> corners;

public:
	QuadSortService(RasterManager* rasterService, MathHelper* mathHelper, CalculationHelper* calculationHelper);

	CalculationQuadResult DoCalculationStep(std::vector<Point> solutionPoints, long currentIndex, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet);

	void reset();

private:
	void createCache(double rasterScale, Picture* picture, AGeometry* facet, std::vector<Point> surfacePoints);

	Point getMinY(std::vector<Point> surfacePoints);
	Point getMinX(std::vector<Point> surfacePoints);
	Point getMaxX(std::vector<Point> surfacePoints);
	Point getMaxY(std::vector<Point> surfacePoints);

	Point getMinYWithSmallestX(std::vector<Point> surfacePoints);

	void addPointsIfNotExists(std::vector<Point>* possiblePointList, std::vector<Point> coveredPoints);

	CalculationQuadResult fillHoles(std::vector<Point> solutionPoints, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet);
	CalculationQuadResult removeUnusedImages(vector<Point> solutionPoints, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet);

	CalculationQuadResult generateFirstStartImage(std::vector<Point> solutionPoints, long currentIndex, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet);
	Point searchXAxeForValidFocus(const Point& point, Picture* picture, AGeometry* facet);
	CalculationQuadResult generateOtherImages(std::vector<Point> solutionPoints, long currentIndex, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet);

	Point getRowBegin(std::vector<Point> currentSolution);
	Point getNearest(std::vector<Point> currentSolution, Point point);
	Point getNearestOnYWithSmallestX(std::vector<Point> currentSolution, double fromY, double toY);
	Point getNearestOnYWithSmallestXInFocus(std::vector<Point> currentSolution, double fromY, double toY, Picture* picture, AGeometry* facet);
	Point getNextLeftXValueInRow(std::vector<Point> surfacePoints, Point point, Picture* picture);
	bool isFirstImageInRow(std::vector<Point> currentSolution);

	void preanalysis(double rasterScale, AGeometry* facet);
	double calculateCoveredScore(Picture* picture, AGeometry* facet, double rasterScale, std::vector<Point> solutionPoints);

	bool isFocusValid(Point currentPosition, Picture* picture, AGeometry* facet);
	std::vector<Point> getInvalidFocusPosition(Point currentPosition, Picture* picture, AGeometry* facet);

	std::vector<Point> generateFirstImagePositionWithMatchingFocus(Picture* picture, AGeometry* facet, double lamda);

	Point moveOnXAxeForValidFocus(Point newCalculatedImage, Point lastImagePoint, Picture* picture, AGeometry* facet);
	Point moveOnYAxeForValidFocus(Point rowStart, Point lastImagePoint, Picture* picture, AGeometry* facet);

	std::vector<Point> getUncoveredPoints(std::vector<Point> surfacePoints, std::vector<Point> coveredPoints);

	std::vector<Point> getNearestInRadius(Point point, const vector<Point>& vector, double radius);
	std::vector<Point> getCorners(AGeometry* facet, double scale, double radius);

	bool checkTopLeft(Picture* picture);
};

#endif
