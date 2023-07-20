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

#include "MonteCarloService.h"

#include <chrono>

MonteCarloService::MonteCarloService(RasterManager* rasterManager, MathHelper* mathHelper, CalculationHelper* calculationHelper)
{
	this->rasterManager = rasterManager;
	this->mathHelper = mathHelper;
    this->calculationHelper = calculationHelper;

    uint64_t ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    srand(ms);
}

std::string MonteCarloService::generateRandomtBinaryCombination(double rasterScale, AGeometry* facet)
{
    string result = "";
    std::vector<Point> surefaceRaster = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);

    for (int i = 0; i < surefaceRaster.size() ;i++)
    {
        int inserValue = this->getRandom(0, 10);

        if(inserValue == 0)
        {
            result += '1';
        }
        else
        {
            result += '0';
        }
    }

	return result;
}

CalculationResult MonteCarloService::calculateScore(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet)
{
    std::vector<Point> surefacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);

    std::vector<Point> imagePoints = this->getPicturePoints(binaryCombination, facet, rasterScale);
    std::vector<Point> coveredPoints = this->rasterManager->calculateCoveredPoints(picture, facet, rasterScale, imagePoints);

    double score = this->calculationHelper->calculateScoreInPercent(coveredPoints.size(), surefacePoints.size());

    return CalculationResult(score, imagePoints);
}

std::string MonteCarloService::mutateCombination(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet)
{
    int range = 10;
    std::vector<Point> surefacePoints = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);
    
    int randomIndex = this->getRandom((range/2), surefacePoints.size() - (range/2));
    int indexBegin = randomIndex - (range/2);

    if(indexBegin < 0)
    {
        indexBegin = 0;
    }

    int indexEnd = randomIndex + (range/2);

    if(indexEnd >= surefacePoints.size())
    {
        indexEnd = surefacePoints.size() - 1;
    }
    
    std::string startBinary = binaryCombination.substr(0, indexBegin);
    std::string endBinary = binaryCombination.substr(indexEnd, surefacePoints.size()-1);
    std::string middelBinary = "";

    for (int i = 0 ; i < indexEnd-indexBegin ; i++)
    {
        int randomValue = this->getRandom(1,2);

        // Insert
        if(randomValue == 1)
        {
            int randomIndex = this->getRandom(1,range);

            std::string begin = binaryCombination.substr(indexBegin, randomIndex-1);
            std::string end = binaryCombination.substr(indexBegin + randomIndex+1, range-randomIndex);

            middelBinary = begin + '1' + end;
        }
        else // Remove
        {
            middelBinary = binaryCombination.substr(indexBegin, indexEnd-indexBegin);
            middelBinary[(range/2)-1] = '0';
        }
    }

    return startBinary + middelBinary + endBinary;
}

std::vector<Point> MonteCarloService::getPicturePoints(std::string binary, AGeometry* facet, double rasterScale)
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

int MonteCarloService::getRandom(int from, int to)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(from, to);

    return dist(mt);
}