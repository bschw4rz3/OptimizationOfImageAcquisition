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

    double minCamWidth = picture->width*0.9;
    double minCamHeight = picture->height*0.9;

    // Create raster
    if(this->rasterCache.size() == 0)
    {
        this->index = 0;

        Point minPoint (this->calculationHelper->getMinX(surefacePoints).x, this->calculationHelper->getMinY(surefacePoints).y);
        Point maxPoint (this->calculationHelper->getMaxX(surefacePoints).x, this->calculationHelper->getMaxY(surefacePoints).y);

        for(double x = minPoint.x - minCamWidth/10 ; x < maxPoint.x + minCamWidth/10 ; x+=minCamWidth)
        {
            for(double y = minPoint.y - minCamHeight/10 ; y < maxPoint.y + minCamHeight/10 ; y+=minCamHeight)
            {
                this->rasterCache.push_back(Point(x + minCamWidth/2, y + minCamHeight/2));
            }
        }
    }
    
    if(this->rasterCache.size()-1 < this->index)
    {
        throw std::exception("Es konnte keine Lösung gefunden werden...");
    }

    Point currentRasterPosition = this->rasterCache[this->index];
    Point currentPoint(0, 0);
    
    int generateImageIndex = 0;
    solutionPoints.push_back(Point(0,0));

    std::vector<Point> coveredPoints;

    Point areaMax(currentRasterPosition.x + minCamWidth/2, currentRasterPosition.y + minCamHeight/2);
    Point areaMin(currentRasterPosition.x - minCamWidth/2, currentRasterPosition.y - minCamHeight/2);

    while(!(this->isFocusValid(currentPoint, picture, facet) && this->covertsAllSegmentPoints(currentPoint, Point(picture->width, picture->height), surefacePoints, areaMax, areaMin)))
    {
        if(generateImageIndex > 10000)
        {
            throw std::exception("Es konnte keine Position gefunden werden...");
        }

        double randomX = this->getRandom(currentRasterPosition.x - minCamWidth, currentRasterPosition.x + minCamWidth);
        double randomY = this->getRandom(currentRasterPosition.y - minCamHeight, currentRasterPosition.y + minCamHeight);
        
        currentPoint = Point(randomX, randomY);
        solutionPoints[solutionPoints.size()-1] = currentPoint;

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

bool GridApproachesService::covertsAllSegmentPoints(Point currentPoint, Point imageDimensions, std::vector<Point> surefacePoints, Point areaMax, Point areaMin)
{
    bool allCovered = true;

    static Point forPosition(-99999, -99999);
    static std::vector<Point> pointCache;

    if(forPosition.x != areaMax.x || forPosition.y != areaMax.y)
    {
        pointCache.clear();

        for (int i = 0; i < surefacePoints.size(); i++)
        {
            Point currentSurfacePoint = surefacePoints[i];

            if(areaMin.x <= currentSurfacePoint.x && currentSurfacePoint.x <= areaMax.x && 
               areaMin.y <= currentSurfacePoint.y && currentSurfacePoint.y <= areaMax.y)
            {
                pointCache.push_back(currentSurfacePoint);
            }
        }

        forPosition = areaMax;
    }

    for (int i = 0; i < pointCache.size(); i++)
    {
        Point currentSurfacePoint = pointCache[i];

        if(!(currentPoint.x - imageDimensions.x/2 <= currentSurfacePoint.x && currentSurfacePoint.x <= currentPoint.x + imageDimensions.x/2 && 
             currentPoint.y - imageDimensions.y/2 <= currentSurfacePoint.y && currentSurfacePoint.y <= currentPoint.y + imageDimensions.y/2))
        {
            allCovered = false;
            break;
        }
    }

    if(allCovered)
    {
        this->conScore += pointCache.size();
    }

    return allCovered;
}

void GridApproachesService::reset()
{
    this->pointCache = std::map<Point, std::vector<Point>>();
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