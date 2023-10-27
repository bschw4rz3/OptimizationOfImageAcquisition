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

#include "CalculationHelper.h"
#include "QuadSortService.h"

CalculationHelper::CalculationHelper(MathHelper* mathHelper)
{
    this->mathHelper = mathHelper;
}

bool CalculationHelper::contains(std::vector<Point> list, Point point)
{
    for (int i = 0; i < list.size(); i++)
    {
        if (this->mathHelper->equals(list[i].x, point.x, DOUBLE_TOLERANCE) 
         && this->mathHelper->equals(list[i].y, point.y, DOUBLE_TOLERANCE))
        {
            return true;
        }
    }

    return false;
}

bool CalculationHelper::contains(std::vector<Point>* list, Point point)
{
    for (int i = 0; i < list->size(); i++)
    {
        if (this->mathHelper->equals(list->at(i).x, point.x, DOUBLE_TOLERANCE) 
         && this->mathHelper->equals(list->at(i).y, point.y, DOUBLE_TOLERANCE))
        {
            return true;
        }
    }

    return false;
}

double CalculationHelper::calculateScoreInPercent(double coveredCount, double totalCount)
{
    return (coveredCount / totalCount);
}

double CalculationHelper::calculateScoreInPercent(std::vector<Point> coveredList, std::vector<Point> totalSurfaceList)
{
    double coveredScore = this->calculateScore(coveredList);
    double totalScore = this->calculateScore(totalSurfaceList);

    return (coveredScore / totalScore);
}

double CalculationHelper::calculateScore(std::vector<Point> pointList)
{
    PointResult coveredCountResult = this->countPoints(pointList);
    PointResult totalCountResult = this->countPoints(pointList);

    double rasterScore = coveredCountResult.rasterPoints * SCORE_WEIGHT_RASTER;
    double boarderScore = coveredCountResult.boarderPoints * SCORE_WEIGHT_BOARDER;

    return rasterScore + boarderScore;
}

PointResult CalculationHelper::countPoints(const vector<Point>& pointVector)
{
    PointResult result;

    result.boarderPoints = 0;
    result.rasterPoints = 0;

    for (int i = 0 ; i < pointVector.size() ; i++)
    {
	    if(this->isRasterPoint(pointVector[i]))
	    {
		    result.rasterPoints++;
	    }
        else
        {
	        result.boarderPoints++;
        }
    }

    return result;
}

bool CalculationHelper::isRasterPoint(Point point)
{
    double rasterDistance = 0.25;

	double restX = std::fmod(point.x, rasterDistance);
	double restY = std::fmod(point.y, rasterDistance);

    bool isValidX = restX == 0;
    bool isValidY = restY == 0;

    return isValidX && isValidY;
}

Point CalculationHelper::getMinYWithSmallestX(std::vector<Point> surfacePoints)
{
    Point minY(0, FLT_MAX);

    for (int i = 0; i < surfacePoints.size(); i++)
    {
        if (minY.y > surfacePoints[i].y)
        {
            minY = surfacePoints[i];
        }

        if (this->mathHelper->equals(minY.y, surfacePoints[i].y, DOUBLE_TOLERANCE) && minY.x > surfacePoints[i].x)
        {
            minY = surfacePoints[i];
        }
    }

    return minY;
}

Point CalculationHelper::getMinY(std::vector<Point> surfacePoints)
{
    Point minY(0, FLT_MAX);

    for (int i = 0; i < surfacePoints.size(); i++)
    {
        if (minY.y > surfacePoints[i].y)
        {
            minY = surfacePoints[i];
        }
    }

    return minY;
}

Point CalculationHelper::getMinX(std::vector<Point> surfacePoints)
{
    Point minX(FLT_MAX, 0);

    for (int i = 0; i < surfacePoints.size(); i++)
    {
        if (minX.x > surfacePoints[i].x)
        {
            minX = surfacePoints[i];
        }
    }

    return minX;
}

Point CalculationHelper::getMaxX(std::vector<Point> surfacePoints)
{
    Point maxX(FLT_MIN, 0);

    for (int i = 0; i < surfacePoints.size(); i++)
    {
        if (maxX.x < surfacePoints[i].x)
        {
            maxX = surfacePoints[i];
        }
    }

    return maxX;
}

Point CalculationHelper::getMaxY(std::vector<Point> surfacePoints)
{
    Point maxY(0, FLT_MIN);

    for (int i = 0; i < surfacePoints.size(); i++)
    {
        if (maxY.y < surfacePoints[i].y)
        {
            maxY = surfacePoints[i];
        }
    }

    return maxY;
}