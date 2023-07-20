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

#include "BruteForceService.h"

BruteForceService::BruteForceService(RasterManager* rasterManager, MathHelper* mathHelper, CalculationHelper* calculationHelper)
{
	this->rasterManager = rasterManager;
	this->mathHelper = mathHelper;
    this->calculationHelper = calculationHelper;
}

std::string BruteForceService::generateFirstBinaryCombination(double rasterScale, AGeometry* facet)
{
	std::vector<Point> surefaceRaster = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);

    return std::string(surefaceRaster.size(), '0');
}

std::string BruteForceService::generateLastBinaryCombination(double rasterScale, AGeometry* facet)
{
    std::vector<Point> surefaceRaster = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);

    return std::string(surefaceRaster.size(), '1');
}

CalculationResult BruteForceService::DoCalculationStep(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet)
{
    std::vector<Point> surefacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);

    std:vector<Point> imagePoints = this->getPicturePoints(binaryCombination, facet, rasterScale);
    std::vector<Point> coveredPoints = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, imagePoints);
    double score = this->calculationHelper->calculateScoreInPercent(coveredPoints.size(), surefacePoints.size());

    return CalculationResult(score, imagePoints);
}

std::vector<Point> BruteForceService::getPicturePoints(std::string binary, AGeometry* facet, double rasterScale)
{
    std::vector<Point> surefacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    std::vector<Point> result;

    for (int i = 0; i < binary.size(); i++)
    {
        char c = binary[i];
        if (c == '1')
        {
            result.push_back(surefacePoints[i]);
        }
    }

    return result;
}