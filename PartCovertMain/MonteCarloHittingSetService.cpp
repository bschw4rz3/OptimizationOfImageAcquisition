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

#include "MonteCarloHittingSetService.h"

// Die Monte-Carlo-Suche ist eine Reihe von zufallsgesteuerten Algorithmen zur Lösung von kombinatorischen Probleme. Während für Zwei-Personen-Spiele wie Go [24]
// baumerkundende Algorithmen wie UCT [16] verwendet werden, hat die verschachtelte Monte-Carlo-Suche [5] bei Ein-Agenten-Spielen zu zu vielen aktuellen Spitzenergebnissen
// geführt. Die Verschachtelung wird als Methode verwendet, um Ausbeutung mit und verdichtet exponentiell die Informationen, die in den rekursiven Aufrufen des Algorithmus
// des Algorithmus. Alle Algorithmen basieren auf dem Konzept des Rollouts, das ein zufälliges Spiel bis zum Ende des Spiels bezeichnet. Solange das Spiel nicht beendet ist,
// wird eine Reihe von Nachfolgern für die Nachfolgern für die aktuelle Position generiert, und ein Nachfolger wird nach dem Zufallsprinzip unter Berücksichtigung einer
// Verstärkungsstrategie ausgewählt, die auf dem Ergebnis der vorangegangenen Durchläufe basiert.

MonteCarloHittingSetService::MonteCarloHittingSetService(RasterManager* rasterService, MathHelper* mathHelper, CalculationHelper* calculationHelper)
{
	this->rasterService = rasterService;
	this->mathHelper = mathHelper;
	this->calculationHelper = calculationHelper;	

	this->beamnrpa = new Beamnrpa();
}

MonteCarloHittingSetService::~MonteCarloHittingSetService()
{
	delete this->beamnrpa;	
}

std::string MonteCarloHittingSetService::generateEmptyBinaryCombination(double rasterScale, AGeometry* facet)
{
    string result = "";
    std::vector<Point> surefaceRaster = this->rasterService->getSurfaceRasterCash(rasterScale, facet);

    for (int i = 0; i < surefaceRaster.size() ;i++)
    {
        result += '0';
    }

	return result;
}

CalculationResult MonteCarloHittingSetService::calculateStep(string currentBinaryCombination, double rasterScale, Picture* picture, AGeometry* facet)
{
    vector<Point> surefacePoints = this->rasterService->getSurfaceRasterCash(rasterScale, facet);

	if(hittingSet.getSize() == 0)
	{
		this->hittingSet = this->descripeHittingSetProblem(rasterScale, picture, facet);
	}

	std::vector<Point> currentPointList = this->getImagePoints(currentBinaryCombination);
	std::vector<Point> currentCovertedList = this->rasterService->calculateCoveredPoints(picture, facet, rasterScale, currentPointList);

    std::vector<Point> bestPointList = currentPointList;
	std::vector<Point> bestCovertedList = currentCovertedList;
    string bestBinaryCombination = currentBinaryCombination;

	for(int i = 0; i < ROLLOUT_SIZE; i++)
	{
        string binaryCombination = currentBinaryCombination+"";
		int changeCount = CHANGE_COUNT;/*this->getRandom(1, MAX_CHANGE_SIZE);*/

        for(int changeIndex = 0; changeIndex < changeCount ;changeIndex++)
        {
			int randIndex = this->getRandom(0, surefacePoints.size());
            Point point = this->hittingSet.fromIndex(randIndex);

            if(this->lastBinaryCombination != currentBinaryCombination)
            {
	            if(!this->calculationHelper->contains(currentPointList, point) && 
                this->calculationHelper->contains(currentCovertedList, point))
	            {
		            changeIndex--;
	                continue;
	            }
            }
            
            if(!this->isFocusValid(point, picture, facet))
            {
	            changeIndex--;
                continue;
            }

            char c = binaryCombination[randIndex];

            if (c == '1')
			{
                c = '0';
            }
            else
            {
	            c = '1';
            }

            binaryCombination[randIndex] = c;
        }

        std::vector<Point> newPointList = this->getImagePoints(binaryCombination);
		std::vector<Point> newCovertedList = this->rasterService->calculateCoveredPoints(picture, facet, rasterScale, newPointList);

        if(newCovertedList.size() > bestCovertedList.size() || 
          (newCovertedList.size() == bestCovertedList.size() && bestPointList.size() < newPointList.size()))
        {
	        bestCovertedList = newCovertedList;
            bestPointList = newPointList;
            bestBinaryCombination = binaryCombination;
        }
	}

    double score = this->calculationHelper->calculateScoreInPercent(bestCovertedList.size(), surefacePoints.size());

    CalculationResult result = CalculationResult(score, bestPointList);
    result.binaryCombination = bestBinaryCombination;

    this->lastBinaryCombination = currentBinaryCombination;

    return result;
}

std::vector<Point> MonteCarloHittingSetService::getImagePoints(const string& binary)
{
	std::vector<Point> pointList;

	for (int i = 0; i < binary.size(); i++)
    {
        char c = binary[i];
        if (c == '1')
        {
            Point point = this->hittingSet.fromIndex(i);
			pointList.push_back(point);
        }
    }

	return pointList;
}

HittingSetDescriptionResult MonteCarloHittingSetService::descripeHittingSetProblem(double rasterScale,	Picture* picture, AGeometry* facet)
{
    HittingSetDescriptionResult result;
	std::vector<Point> surfacePoints = this->rasterService->getSurfaceRasterCash(rasterScale, facet);

    for (Point currentPoint : surfacePoints)
    {
	    std::vector<Point> covertedList = this->rasterService->calculateCoveredPoint(currentPoint, picture, facet, rasterScale);
        result.add(HittingSetPoint(currentPoint, covertedList));
    }

    return result;
}

int MonteCarloHittingSetService::getRandom(int from, int to)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(from, to);

    return dist(mt);
}

bool MonteCarloHittingSetService::isFocusValid(Point currentPosition, Picture* picture, AGeometry* facet)
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

void MonteCarloHittingSetService::reset()
{
	this->hittingSet = HittingSetDescriptionResult();
    this->lastBinaryCombination = "";
}