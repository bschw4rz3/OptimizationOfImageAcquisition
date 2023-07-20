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

#ifndef MonteCarloHittingSetSerivce_H
#define MonteCarloHittingSetSerivce_H

#include <map>
#include <random>

#include "../PartCover/RasterManager.h"
#include "../PartCover/MathHelper.h"
#include "../PartCover/Picture.h"
#include "CalculationResult.h"
#include "CalculationHelper.h"
#include "HittingSetDescriptionResult.h"

#include "../StefansHittingSolver/beamnrpa.h"

#define ROLLOUT_SIZE 50
#define CHANGE_COUNT 1

class MonteCarloHittingSetService
{
private:
	std::string lastBinaryCombination;

private:
	RasterManager* rasterService;
	MathHelper* mathHelper;
	CalculationHelper* calculationHelper;

	Beamnrpa* beamnrpa;

	HittingSetDescriptionResult hittingSet;

public:
	MonteCarloHittingSetService(RasterManager* rasterService, MathHelper* mathHelper, CalculationHelper* calculationHelper);
	~MonteCarloHittingSetService();

	std::string generateEmptyBinaryCombination(double rasterScale, AGeometry* facet);
	CalculationResult calculateStep(std::string newBinaryCombination, double rasterScale, Picture* picture, AGeometry* facet);
	std::string mutateCombination(const std::string& string, double raster_scale, Picture* picture, AGeometry* facet);
	void reset();

private:
	HittingSetDescriptionResult descripeHittingSetProblem(double raster_scale, Picture* picture, AGeometry* facet);
	std::vector<Point> getImagePoints(const std::string& currentBinaryCombination);
	int getRandom(int from, int to);

	bool isFocusValid(Point currentPosition, Picture* picture, AGeometry* facet);
};

#endif

