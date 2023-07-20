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

#include "RasterManager.h"

RasterManager::RasterManager(RasterService* rasterService)
{
	this->rasterService = rasterService;

    this->facetCache = NULL;
    this->rasterScaleCache = 0;
}

std::vector<Point> RasterManager::calculateCoveredPoints(Picture* picture, AGeometry* facet, double rasterScale, std::vector<Point> imagePoints)
{
    std::vector<Point> surefacePoints = this->getSurfaceRasterCash(rasterScale, facet);
    
    // Performance init:
    #ifdef NPERFOMANCE
    std::vector<std::vector<Point>> coveredPointsList;
	#else
	std::vector<Point> results;
	#endif

    for (int i = 0; i < imagePoints.size(); i++)
    {
        Point imagePoint = imagePoints[i];

        std::vector<Point> coveredPoints = this->calculateCoveredPoint(imagePoint, picture, facet, rasterScale);

        // Performance statements for: coveredPointsList.push_back(coveredPoints);
#ifdef NPERFOMANCE
        coveredPointsList.push_back(coveredPoints);
#else
        results.reserve(results.size() + coveredPoints.size());
        results.insert(results.end(), coveredPoints.begin(), coveredPoints.end());
#endif 
    }

    // Performance statements for: return this->toUniquePointList(coveredPointsList);
#ifdef NPERFOMANCE
    return this->toUniquePointList(coveredPointsList);
#else
    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());

    return results;
#endif    
}

std::vector<Point> RasterManager::calculateCoveredPoints(Picture* picture, AGeometry* facet, double rasterScale, Point imagePoint)
{
    std::vector<Point> surefacePoints = this->getSurfaceRasterCash(rasterScale, facet);
    std::vector<std::vector<Point>> coveredPointsList;

    std::vector<Point> coveredPoints = this->calculateCoveredPoint(imagePoint, picture, facet, rasterScale);
    coveredPointsList.push_back(coveredPoints);

    return this->toUniquePointList(coveredPointsList);
}

std::vector<Point> RasterManager::calculateCoveredPoint(Point currentPoint, Picture* picture, AGeometry* facet, double rasterScale)
{
	std::vector<Point> surefacePoints = this->getSurfaceRasterCash(rasterScale, facet);

    // Performance init:
    std::vector<Point> coveredPoints;

    for (int i = 0; i < surefacePoints.size(); i++)
    {
        Point current = surefacePoints[i];
        
        if (this->pointIsInPicture(currentPoint, picture, current))
        {
            coveredPoints.push_back(surefacePoints[i]);
        }
    }

    return coveredPoints;
}

bool RasterManager::pointIsInPicture(Point picturePosition, Picture* picture, Point current)
{
    double halfWidth = picture->width / 2;
    double halfHeigth = picture->height / 2;

    double boarderLowX = picturePosition.x - halfWidth;
    double boarderHeighX = picturePosition.x + halfWidth;
    double boarderLowY = picturePosition.y - halfHeigth;
    double boarderHeighY = picturePosition.y + halfHeigth;

	return boarderLowX <= current.x && boarderHeighX >= current.x &&
           boarderLowY <= current.y && boarderHeighY >= current.y;
}

std::vector<Point> RasterManager::getSurfaceRasterCash(double rasterScale, AGeometry* facet)
{
    if (this->surefaceRasterScaleCache != rasterScale || this->surefaceFacetCache != facet)
    {
        this->surefaceRasterScaleCache = rasterScale;
        this->surefaceFacetCache = facet;

        RasterResult raster = this->rasterService->calculateBoarderRaster(rasterScale, facet, RASTER_TRITACTIC);
        this->surefaceRasterCash = raster.getSurefaceRasterPoints();
    }

    return this->surefaceRasterCash;
}

std::vector<Point> RasterManager::getRasterCash(double rasterScale, AGeometry* facet)
{
    if (this->rasterScaleCache != rasterScale || this->facetCache != facet)
    {
        this->rasterScaleCache = rasterScale;
        this->facetCache = facet;

        RasterResult raster = this->rasterService->calculateBoarderRaster(rasterScale, facet, RASTER_TRITACTIC);

        vector<Point> surfaceRaster = raster.getSurefaceRasterPoints();
        vector<Point> backgroundRaster = raster.getBackgroundRasterPoints();

        std::vector<std::vector<Point>> listList;
        listList.push_back(surfaceRaster);
        listList.push_back(backgroundRaster);

        this->rasterCash = this->toUniquePointList(listList);
    }

    return this->rasterCash;
}

std::vector<Point> RasterManager::toUniquePointList(std::vector<std::vector<Point>> listList)
{
    std::vector<Point> uniqueList;

    for (int x = 0; x < listList.size(); x++)
    {
        for (int y = 0; y < listList[x].size(); y++)
        {
            if (std::find(uniqueList.begin(), uniqueList.end(), listList[x][y]) == uniqueList.end())
            {
                // someName not in name, add it
                uniqueList.push_back(listList[x][y]);
            }
        }
    }

    return uniqueList;
}