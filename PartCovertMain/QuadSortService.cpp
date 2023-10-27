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

#include "QuadSortService.h"

double QuadSortService::angleDetectionRadius = 0.25;
int QuadSortService::minAngle = 60;

int CanNotAddNewRowException = 1;

QuadSortService::QuadSortService(RasterManager* rasterService, MathHelper* mathHelper, CalculationHelper* calculationHelper)
{
	this->rasterManager = rasterService;
	this->mathHelper = mathHelper;
    this->calculationHelper = calculationHelper;

    this->contourMarker = Point(NAN, NAN);
    this->topLeftCorner = Point(NAN, NAN);
    this->holeFillingMode = false;
}

CalculationQuadResult QuadSortService::DoCalculationStep(std::vector<Point> solutionPoints, long currentIndex, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet)
{
    if(!this->holeFillingMode)
    {
	    try
	    {
		    this->preanalysis(rasterScale, facet);

		    if (possibleStartPoints.size() == 0)
		    {
		        possibleStartPoints.push_back(this->contourMarker);
		    }
		        
		    if (currentIndex < possibleStartPoints.size())
		    {
		        return this->generateFirstStartImage(solutionPoints, currentIndex, possibleStartPoints, rasterScale, picture, facet);
		    }
		    else if(currentIndex >= possibleStartPoints.size() && solutionPoints.size() == 0)
		    {
			    std::vector<Point> newPossiblePoints = this->generateFirstImagePositionWithMatchingFocus(picture, facet, 0.05);

		        if(newPossiblePoints.size() == 0)
		        {
			        throw exception("Can't find a starting position. (Focuspoints?)");
		        }

		        possibleStartPoints.push_back(Point(0,0));

		        for(int i = 0 ; i < newPossiblePoints.size() ;i++)
		        {
			        possibleStartPoints.push_back(newPossiblePoints[i]);
		        }

		        return CalculationQuadResult(0, std::vector<Point>(), possibleStartPoints);
		    }
		    else
		    {
		        return this->generateOtherImages(solutionPoints, currentIndex, possibleStartPoints, rasterScale, picture, facet);
		    }
	    }
	    catch(int e)
	    {
		    if(e == CanNotAddNewRowException)
		    {
	            this->holeFillingMode = true;
		    }
	        else
	        {
		        throw e;
	        }
	    }
    }
    
    if(this->holeFillingMode)
    {
        CalculationQuadResult result = this->fillHoles(solutionPoints, possibleStartPoints, rasterScale, picture, facet);

        if(result.coverRate >= 1.0)
        {
	        result = this->removeUnusedImages(result.coveredPoints, possibleStartPoints, rasterScale, picture, facet);
        }

        return result;
    }
}

CalculationQuadResult QuadSortService::generateFirstStartImage(std::vector<Point> solutionPoints, long currentIndex, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet)
{
    Point current = possibleStartPoints[currentIndex];

    std::vector<Point> surefacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    std::vector<Point> coveredPoints = this->rasterManager->calculateCoveredPoint(current, picture, facet, rasterScale);

    Point nearestLeftBoarderPoint = this->getNearestOnYWithSmallestX(surefacePoints, current.y - picture->height * 0.5, current.y + picture->height * 0.5);
    
    bool isContourInPicture = this->rasterManager->pointIsInPicture(current, picture, this->contourMarker);
    bool isTopLeftCornerInPicture = (isnan(this->topLeftCorner.x) || (!isnan(this->topLeftCorner.x) && this->rasterManager->pointIsInPicture(current, picture, this->topLeftCorner)));

    if (isContourInPicture && (isTopLeftCornerInPicture || !this->checkTopLeft(picture)))
    {
        this->addPointsIfNotExists(&possibleStartPoints, coveredPoints);

        std::vector<Point> startPointList;
        startPointList.push_back(current);

		bool isNearestPointCoverted = this->calculationHelper->contains(coveredPoints, nearestLeftBoarderPoint);

        if(this->contourMarker.x > nearestLeftBoarderPoint.x && isNearestPointCoverted)
        {
	        this->contourMarker.x = nearestLeftBoarderPoint.x;
        }

        double score = 0;
        
        if(this->isFocusValid(current, picture, facet))
        {
	        score = this->calculationHelper->calculateScoreInPercent(coveredPoints, surefacePoints);
        }

        return CalculationQuadResult(score, startPointList, possibleStartPoints);
    }
    
    return CalculationQuadResult(0, solutionPoints, possibleStartPoints);
}

Point QuadSortService::searchXAxeForValidFocus(const Point& point, Picture* picture, AGeometry* facet)
{
    double optimizedX = NAN;

    for(double x = point.x - picture->width*0.5 ; x < point.x + picture->width*0.5 ; x+=0.05)
    {
	    if(this->isFocusValid(Point(x, point.y), picture, facet))
	    {
	    	optimizedX = x;
	    }
    }

    if(isnan(optimizedX))
    {
	    throw CanNotAddNewRowException;
    }

    return Point(optimizedX, point.y);
}

bool QuadSortService::checkTopLeft(Picture* picture)
{
	double distance = this->mathHelper->distance(this->contourMarker, this->topLeftCorner);

    return distance < picture->width;
}

CalculationQuadResult QuadSortService::generateOtherImages(std::vector<Point> solutionPoints, long currentIndex, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet)
{
    bool addNewRows = true;
    std::vector<Point> surfacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    std::vector<Point> rasterPoints = this->rasterManager->getRasterCash(rasterScale, facet);

	// Try to move the image to get a better first position in row
    if(this->isFirstImageInRow(solutionPoints))
    {
        Point currentImagePoint = solutionPoints[solutionPoints.size()-1];
        std::vector<Point> currentCoveredPoints = this->rasterManager->calculateCoveredPoint(currentImagePoint, picture, facet, rasterScale);
        
        Point newImagePoint = this->getNextLeftXValueInRow(surfacePoints, currentImagePoint, picture);

        if(this->isFocusValid(newImagePoint, picture, facet))
        {
	        std::vector<Point> newCoveredPoints = this->rasterManager->calculateCoveredPoint(newImagePoint, picture, facet, rasterScale);

		    if (this->contourMarker.x + (picture->width/2) >= newImagePoint.x && 
	            currentCoveredPoints.size() < newCoveredPoints.size())
		    {
	            //if(!this->mathHelper->equals(solutionPoints[solutionPoints.size()-1].x, newImagePoint.x, DOUBLE_TOLERANCE) || 
	            //   !this->mathHelper->equals(solutionPoints[solutionPoints.size()-1].y, newImagePoint.y, DOUBLE_TOLERANCE))
	            {
		            solutionPoints[solutionPoints.size()-1].x = newImagePoint.x;
		            solutionPoints[solutionPoints.size()-1].y = newImagePoint.y;
		            addNewRows = false;
	            }
	        }
        }
    }

    double currentScore = this->calculateCoveredScore(picture, facet, rasterScale, solutionPoints);

    if(addNewRows)
    {
        // Try to add a new Image in on current row
	    Point lastImagePoint = solutionPoints[solutionPoints.size()-1];

        double rasterWidth = picture->width;

        Point newCalculatedImage(lastImagePoint.x + rasterWidth, lastImagePoint.y);
        newCalculatedImage = this->moveOnXAxeForValidFocus(newCalculatedImage, lastImagePoint, picture, facet);
        
        solutionPoints.push_back(newCalculatedImage);

        double newScore = this->calculateCoveredScore(picture, facet, rasterScale, solutionPoints);

        // Wenn nicht gut, dann neue Zeile hinzufügen
	    if(currentScore >= newScore)
	    {
		    solutionPoints.pop_back();

	        Point lastRowBegin = this->getRowBegin(solutionPoints);
	        Point nearestSurfacePoint = this->getNearestOnYWithSmallestX(surfacePoints, lastRowBegin.y + picture->height * 0.5, lastRowBegin.y + picture->height * 1.5);
            
	        Point rowStart = Point(nearestSurfacePoint.x + (picture->width* 0.45), lastRowBegin.y + picture->height);

            try
            {
	            rowStart = this->moveOnYAxeForValidFocus(rowStart, lastImagePoint, picture, facet);
            }
            catch(int)
            {
	            rowStart = this->searchXAxeForValidFocus(rowStart, picture, facet);
            }
            
            this->contourMarker = nearestSurfacePoint;
	        solutionPoints.push_back(rowStart);

            newScore = this->calculateCoveredScore(picture, facet, rasterScale, solutionPoints);

            if(currentScore >= newScore)
            {
	            throw CanNotAddNewRowException;
            }
	    }

	    currentScore = this->calculateCoveredScore(picture, facet, rasterScale, solutionPoints);
    }
    
    double score = this->calculationHelper->calculateScoreInPercent(currentScore, this->calculationHelper->calculateScore(surfacePoints));
    
    return CalculationQuadResult(score, solutionPoints, possibleStartPoints);
}

CalculationQuadResult QuadSortService::fillHoles(std::vector<Point> solutionPoints, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet)
{
    double approximationFactor = 0.1;
    int maxIterations = 1500;

	std::vector<Point> surfacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    std::vector<Point> coveredPoints = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);
	std::vector<Point> restPoints = this->getUncoveredPoints(surfacePoints, coveredPoints);

	int maxScore = 0;
    Point maxPoint = Point(NAN, NAN);

    for(int i = 0; i < restPoints.size() ; i++)
    {
	    Point currentPoint = restPoints[i];

        if(this->isFocusValid(currentPoint, picture, facet))
        {
	        solutionPoints.push_back(currentPoint);

            std::vector<Point> coveredResult = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);

        	if(maxScore < coveredResult.size())
            {
                maxScore = coveredResult.size();
                maxPoint = currentPoint;
            }

            solutionPoints.pop_back();
        }
        else if(i < maxIterations)
        {
	        std::vector<Point> focusPointList = this->getInvalidFocusPosition(currentPoint, picture, facet);

            for(int h = 0;h < focusPointList.size();h++)
            {
	            Point focusPoint = focusPointList[h];
                Point delta = Point(focusPoint.x - currentPoint.x, focusPoint.y - currentPoint.y);

	            Point newPoint = Point(currentPoint.x - (delta.x*approximationFactor), currentPoint.y - (delta.y*approximationFactor));
	            restPoints.push_back(newPoint);
            }
        }
    }

    if(isnan(maxPoint.x) || isnan(maxPoint.x))
    {
	    throw exception("can not detect holes!");
    }

    solutionPoints.push_back(maxPoint);
    std::vector<Point> coveredResult = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);
    double score = this->calculationHelper->calculateScoreInPercent(coveredResult, surfacePoints);

    return CalculationQuadResult(score, solutionPoints, possibleStartPoints);
}

CalculationQuadResult QuadSortService::removeUnusedImages(vector<Point> solutionPoints, std::vector<Point> possibleStartPoints, double rasterScale, Picture* picture, AGeometry* facet)
{
    std::vector<Point> absoluteCovers = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);

	for(int i = 0; i < solutionPoints.size() ;i++)
	{
		Point currentPoint = solutionPoints[i];

        solutionPoints[i].x = NAN;
        solutionPoints[i].y = NAN;

        std::vector<Point> coveredResult = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);

        if(absoluteCovers.size() > coveredResult.size())
        {
	        solutionPoints[i].x = currentPoint.x;
			solutionPoints[i].y = currentPoint.y;
        }
	}

    for(int i = 0; i < solutionPoints.size() ;i++)
	{
        if(isnan(solutionPoints[i].x))
        {
	        solutionPoints.erase(solutionPoints.begin() + i);
            i--;
        }
    }

    std::vector<Point> surfacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    std::vector<Point> coveredResult = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);

    double score = this->calculationHelper->calculateScoreInPercent(coveredResult, surfacePoints);
    return CalculationQuadResult(score, solutionPoints, possibleStartPoints);
}

Point QuadSortService::moveOnXAxeForValidFocus(Point newCalculatedImage, Point lastImagePoint, Picture* picture, AGeometry* facet)
{
	double rasterStep = picture->width/100.0;

    while(!this->isFocusValid(newCalculatedImage, picture, facet))
    {
        if(newCalculatedImage.x < lastImagePoint.x + (picture->width/2))
        {
            //throw exception("Can't add image to row. No Focus found!");

            // Kann kein Row hinzufügen, erstmal weiter machen... (score gleich halten)
            newCalculatedImage = lastImagePoint;
            break;
        }

        newCalculatedImage.x-=rasterStep;
    }

    return newCalculatedImage;
}

Point QuadSortService::moveOnYAxeForValidFocus(Point rowStart, Point lastImagePoint, Picture* picture, AGeometry* facet)
{
	double rasterStep = picture->height/100.0;

    while(!this->isFocusValid(rowStart, picture, facet))
    {
        if(rowStart.y < lastImagePoint.y)
        {
            throw CanNotAddNewRowException;
        }

        rowStart.y-=rasterStep;
    }

    return rowStart;
}

Point QuadSortService::getNextLeftXValueInRow(std::vector<Point> surfacePoints, Point point, Picture* picture)
{
    Point current;
    double minDistanceX = FLT_MAX;
    double halfHeight = picture->height/2;

	for (int i = 0 ; i < surfacePoints.size() ; i++)
	{
        if(surfacePoints[i].y < point.y - halfHeight || surfacePoints[i].y > point.y + halfHeight)
        {
	        continue;
        }

        double distance = surfacePoints[i].x - point.x;

		if(distance > 0 && distance < minDistanceX)
		{
            current.x = surfacePoints[i].x;
            current.y = point.y;

			minDistanceX = distance;
		}
	}

    return current;
}

bool QuadSortService::isFirstImageInRow(std::vector<Point> currentSolution)
{
    Point beginOfRow = this->getRowBegin(currentSolution);

    for(int i = 0; i < currentSolution.size() ;i++)
	{
        if(currentSolution[i] == beginOfRow)
        {
	        return i == currentSolution.size()-1;
        }
    }

    return false;
}

Point QuadSortService::getRowBegin(std::vector<Point> currentSolution)
{
    if(currentSolution.size() == 0)
    {
	    throw exception("No first image found!");
    }

	Point lastRowPoint = currentSolution[currentSolution.size()-1];

    for(int i = currentSolution.size()-2; i >= 0 ;i--)
    {
	    if(lastRowPoint.y != currentSolution[i].y)
	    {
		    return currentSolution[i+1];
	    }
    }

    return currentSolution[0];
}

Point QuadSortService::getNearest(std::vector<Point> currentSolution, Point point)
{
    double minDistance = FLT_MAX;
    Point minPoint;

	for(int i = 0; i < currentSolution.size() ;i++)
	{
        if(!this->calculationHelper->isRasterPoint(currentSolution[i]))
        {
	        continue;
        }

        double distance = this->mathHelper->distance(currentSolution[i], point);
        //double distance = sqrt(pow(currentSolution[i].x - point.x, 2) + pow(currentSolution[i].y - point.y, 2) * 1.0);

		if(minDistance > distance)
		{
			minPoint = currentSolution[i];
            minDistance = distance;
		}
	}

    return minPoint;
}

std::vector<Point> QuadSortService::getCorners(AGeometry* facet, double scale, double radius)
{
    std::vector<Point> result;
    std::vector<Point> contour = facet->getContour(scale);

    for(int i = 0; i < contour.size();i++)
    {
        double modifyedRadius = radius;
        std::vector<Point> nearestList;

        while(nearestList.size() < 2)
        {
	        nearestList.clear();
            nearestList = this->getNearestInRadius(contour[i], contour, modifyedRadius);
            modifyedRadius += 0.01;

            if(nearestList.empty())
            {
	            break;
            }
        }

        if(nearestList.empty())
        {
            break;
        }

        Point firstContourPoint = nearestList[0];
        Point lastContourPoint = nearestList[nearestList.size()-1];

        double angle = this->mathHelper->getAngle(firstContourPoint, contour[i], lastContourPoint);

        if(angle <= minAngle)
        {
	        result.push_back(contour[i]);
        }
    }

    // Performance statements for: Unique
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

std::vector<Point> QuadSortService::getNearestInRadius(Point point, const vector<Point>& vector, double radius)
{
    std::vector<Point> result;

	for(int i = 0; i < vector.size() ;i++)
	{
		if((vector[i].x < point.x + radius && vector[i].x > point.x - radius) 
        && (vector[i].y < point.y + radius && vector[i].y > point.y - radius)
        && !(this->mathHelper->equals(point.x, vector[i].x, DOUBLE_TOLERANCE)
        &&   this->mathHelper->equals(point.y, vector[i].y, DOUBLE_TOLERANCE)))
		{
			double distance = this->mathHelper->distance(vector[i], point);

            if(distance < radius)
            {
	            result.push_back(vector[i]);
            }
		}
	}

    return result;
}

Point QuadSortService::getNearestOnYWithSmallestX(std::vector<Point> currentSolution, double fromY, double toY)
{
    double minX = FLT_MAX;
    Point minPoint(FLT_MAX, FLT_MAX);

	for(int i = 0; i < currentSolution.size() ;i++)
	{
        if(fromY <= currentSolution[i].y && currentSolution[i].y <= toY)
        {
			if(currentSolution[i].x < minX)
			{
				minPoint = currentSolution[i];
	            minX = currentSolution[i].x;
			}
        }
	}

    return minPoint;
}

Point QuadSortService::getNearestOnYWithSmallestXInFocus(std::vector<Point> currentSolution, double fromY, double toY, Picture* picture, AGeometry* facet)
{
    double minX = FLT_MAX;
    Point minPoint(FLT_MAX, FLT_MAX);

	for(int i = 0; i < currentSolution.size() ;i++)
	{
        if(fromY <= currentSolution[i].y && currentSolution[i].y <= toY)
        {
			if(currentSolution[i].x < minX && this->isFocusValid(currentSolution[i], picture, facet))
			{
				minPoint = currentSolution[i];
	            minX = currentSolution[i].x;
			}
        }
	}

    return minPoint;
}

/*
 * Get lowest Point in the top-left corner
 */
void QuadSortService::preanalysis(double rasterScale, AGeometry* facet)
{
    std::vector<Point> surefacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    
    if (isnan(this->contourMarker.x) || isnan(this->contourMarker.y))
	{
	    if(this->corners.empty())
	    {
		    this->corners = this->getCorners(facet, rasterScale, angleDetectionRadius);
	    }  

		this->contourMarker = this->calculationHelper->getMinYWithSmallestX(surefacePoints);

	    if(!this->corners.empty())
	    {
		    this->topLeftCorner = this->calculationHelper->getMinYWithSmallestX(this->corners);
	    }
	}
}

void QuadSortService::addPointsIfNotExists(std::vector<Point>* possiblePointList, std::vector<Point> coveredPoints)
{
    for (int i = 0; i < coveredPoints.size(); i++)
    {
        if (!this->calculationHelper->contains(possiblePointList, coveredPoints[i]))
        {
            possiblePointList->push_back(coveredPoints[i]);
        }
    }
}

void QuadSortService::createCache(double rasterScale, Picture* picture, AGeometry* facet, std::vector<Point> surfacePoints)
{
    if (this->pointCache.size() == 0)
    {
        for (int i = 0; i < surfacePoints.size(); i++)
        {
            Point currentPoint = surfacePoints[i];

            std::vector<Point> currentPointList;
            currentPointList.push_back(currentPoint);

            std::vector<Point> coveredPoints = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, currentPointList);
            this->pointCache[currentPoint] = coveredPoints;
        }
    }
}

double QuadSortService::calculateCoveredScore(Picture* picture, AGeometry* facet, double rasterScale, std::vector<Point> solutionPoints)
{
	std::vector<Point> points = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, solutionPoints);
	return this->calculationHelper->calculateScore(points);
}

bool QuadSortService::isFocusValid(Point currentPosition, Picture* picture, AGeometry* facet)
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

std::vector<Point> QuadSortService::getInvalidFocusPosition(Point currentPosition, Picture* picture, AGeometry* facet)
{
    std::vector<Point> resultList;

    if(!picture->enableFocusPoints)
    {
	    return resultList;
    }

    std::vector<Point> focusList = picture->getRelativFocusPoints();

    for(int i = 0; i < focusList.size(); i++)
    {
        Point relativFocusPoint = focusList[i];
        Point absoluteFocusPoint(currentPosition.x + relativFocusPoint.x, currentPosition.y + relativFocusPoint.y);

	    bool isValid = facet->isOnSurface(Point(absoluteFocusPoint.y, absoluteFocusPoint.x));

        if(!isValid)
        {
	        resultList.push_back(absoluteFocusPoint);
        }
    }

    return resultList;
}

std::vector<Point> QuadSortService::generateFirstImagePositionWithMatchingFocus(Picture* picture, AGeometry* facet, double lamda)
{
    std::vector<Point> result;
    std::vector<Point> relativeFocusPoints = picture->getRelativFocusPoints();

    for(double x = this->contourMarker.x ; x < this->contourMarker.x + (picture->width*0.5) ; x += lamda )
    {
	    for(double y = this->contourMarker.y ; y < this->contourMarker.y + (picture->height*0.5) ; y += lamda )
	    {
            bool isValidPosition = true;

		    for(int i = 0 ; i < relativeFocusPoints.size() ; i++)
		    {
			    double relativeX = relativeFocusPoints[i].x;
			    double relativeY = relativeFocusPoints[i].y;

                Point absoluteFocusPoint(x + relativeX, y + relativeY);

                if(!facet->isOnSurface(Point(absoluteFocusPoint.y, absoluteFocusPoint.x)))
                {
                    isValidPosition = false;
	                break;
                }
		    }

            if(isValidPosition)
            {
	            result.push_back(Point(x,y));
            }
	    }
    }

	return result;
}

std::vector<Point> QuadSortService::getUncoveredPoints(std::vector<Point> surfacePoints, std::vector<Point> coveredPoints)
{
    std::vector<Point> restPoints;

	for(int i = 0 ; i < surfacePoints.size() ; i++)
	{
		Point currentPoint = surfacePoints[i];

		if(!this->calculationHelper->contains(coveredPoints, currentPoint))
		{
			restPoints.push_back(currentPoint);
		}
	}

    return restPoints;
}

void QuadSortService::reset()
{
	this->corners = std::vector<Point>();
    this->topLeftCorner = Point(NAN, NAN);
    this->contourMarker = Point(NAN, NAN);
    this->pointCache = std::map<Point, std::vector<Point>>();
    this->holeFillingMode = false;
}