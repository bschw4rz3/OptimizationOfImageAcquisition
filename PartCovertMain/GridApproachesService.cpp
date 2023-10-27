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

#include "GridApproachesService.h"

GridApproachesService::GridApproachesService(RasterManager* rasterManager, MathHelper* mathHelper, CalculationHelper* calculationHelper)
{
    this->rasterManager = rasterManager;
    this->mathHelper = mathHelper;
    this->calculationHelper = calculationHelper;
}

CalculationResult GridApproachesService::DoCalculationStep(std::vector<Point> solutionPoints, double rasterScale, Picture* picture, AGeometry* facet)
{
    std::vector<Point> surefacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    std::vector<Point> points = this->getUnusedPoints(surefacePoints, solutionPoints);

    // Create raster
    if(this->rasterCache.size() == 0)
    {
        this->index = 0;

        Point minPoint (this->calculationHelper->getMinX(surefacePoints).x, this->calculationHelper->getMinY(surefacePoints).y);
        Point maxPoint (this->calculationHelper->getMaxX(surefacePoints).x, this->calculationHelper->getMaxY(surefacePoints).y);

        for(double x = minPoint.x; x < maxPoint.x ; x+=picture->width)
        {
            for(double y = minPoint.y; y < maxPoint.y ; y+=picture->height)
            {
                this->rasterCache.push_back(Point(x + picture->width/2, y + picture->height/2));
            }
        }
    }
    
    if(this->rasterCache.size()-1 < this->index)
    {
        throw std::exception("Es konnte keine Lösung gefunden werden...");
    }

    Point currentRasterPosition = this->rasterCache[this->index];
    Point currentPoint(currentRasterPosition.x, currentRasterPosition.y);
    
    int generateImageIndex = 0;
    solutionPoints.push_back(currentPoint);

    std::vector<Point> coveredPoints;

    while(!this->isFocusValid(currentPoint, picture, facet))
    {
        if(generateImageIndex > 1000)
        {
            throw std::exception("Es konnte keine Position gefunden werden...");
        }

        double randomX = this->getRandom(currentRasterPosition.x - picture->width, currentRasterPosition.x + picture->width);
        double randomY = this->getRandom(currentRasterPosition.y - picture->height, currentRasterPosition.y + picture->height);
        
        currentPoint = Point(randomX, randomY);
        solutionPoints[solutionPoints.size()-1] = currentPoint;
        
        coveredPoints = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);

        generateImageIndex++;
    }

    coveredPoints = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);

    this->index++;

    double score = this->calculationHelper->calculateScoreInPercent(coveredPoints.size(), surefacePoints.size());

    if (score >= 1.0)
    {
        for (int i = 0; i < coveredPoints.size(); i++)
        {
            if (!this->calculationHelper->contains(surefacePoints, coveredPoints[i]))
            {
                throw exception("Invalid solution");
            }
        }
    }

    return CalculationResult(score, solutionPoints);
}

void GridApproachesService::reset()
{
    this->pointCache = std::map<Point, std::vector<Point>>();
}

Point GridApproachesService::getNextHighest(double rasterScale, Picture* picture, AGeometry* facet, std::vector<Point> surefacePoints, std::vector<Point> coveredPoints)
{
    this->createCache(rasterScale, picture, facet, surefacePoints);

    int max = 0;
    Point maxPoint;

    for (int i = 0; i < surefacePoints.size(); i++)
    {
        Point currentPoint = surefacePoints[i];
        std::vector<Point> pointList = this->getCoveredPoints(currentPoint);
        std::vector<Point> uncoveredPoint = this->getUnusedPoints(pointList, coveredPoints);

        if (uncoveredPoint.size() > max && 
            this->isFocusValid(currentPoint, picture, facet))
        {
            max = uncoveredPoint.size();
            maxPoint = currentPoint;
        }
    }

    if (max == 0)
    {
        throw std::exception("GreedyExtendedService: Kein Punkt gefunden welcher eine abdeckende wirkung hat...");
    }

    return maxPoint;
}

void GridApproachesService::createCache(double rasterScale, Picture* picture, AGeometry* facet, std::vector<Point> surefacePoints)
{
    if (this->pointCache.size() == 0)
    {
        for (int i = 0; i < surefacePoints.size(); i++)
        {
            Point currentPoint = surefacePoints[i];

            std::vector<Point> currentPointList;
            currentPointList.push_back(currentPoint);

            std::vector<Point> coveredPoints = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, currentPointList);
            this->pointCache[currentPoint] = coveredPoints;
        }
    }
}

std::vector<Point> GridApproachesService::getUnusedPoints(std::vector<Point> allPoints, std::vector<Point> usedPoint)
{
    std::vector<Point> result;

    for (int i = 0; i < allPoints.size(); i++)
    {
        if (!this->calculationHelper->contains(usedPoint, allPoints[i]))
        {
            result.push_back(allPoints[i]);
        }
    }

    return result;
}

std::vector<Point> GridApproachesService::getCoveredPoints(Point point)
{
    return this->pointCache[point];
}

bool GridApproachesService::isFocusValid(Point currentPosition, Picture* picture, AGeometry* facet)
{
    if(!picture->enableFocusPoints)
    {
	    return true;
    }

    std::vector<Point> focusList = picture->getRelativFocusPoints();

    for(int i = 0; i < focusList.size(); i++)
    {
        Point relativFocusPoint = focusList[i];
        Point absoluteFocusPoint(currentPosition.x + relativFocusPoint.x, currentPosition.y + relativFocusPoint.y);

	    bool isValid = facet->isOnSurface(Point(absoluteFocusPoint.y, absoluteFocusPoint.x));

        if(!isValid)
        {
	        return false;
        }
    }

    return true;
}

double GridApproachesService::getRandom(double from, double to)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(from, to);

    return dist(mt);
}