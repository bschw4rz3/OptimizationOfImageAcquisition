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

#ifndef MyEventReceiver_H
#define MyEventReceiver_H

#include <activation.h>
#include "SimulationResult.h"
#include <iostream>
#include <fstream>

#include <thread>
#include <future>

#include <irrlicht.h>
#include "driverChoice.h"

#include "../IrrlichtWrapper/GraphicEngine.h"
#include "../IrrlichtWrapper/SAppContext.h"
#include "../IrrlichtWrapper/EventReceiver.h"
#include "../PartCover/Picture.h"
#include "../PartCover/BinaryService.h"
#include "BananaPrimitive.h"

#include "BruteForceService.h"
#include "MonteCarloService.h"
#include "GridApproachesService.h"
#include "GreedyService.h"
#include "MonteCarloHittingSetService.h"
#include "QuadSortService.h"

#define stringify( name ) #name

enum {
    GUI_ID_FORCE,
    GUI_ID_GREEDY,
    GUI_ID_QUADSEARCH,
    GUI_ID_SEQUENCE_MONTE_CARLO,
    GUI_ID_ROLLOUT_MONTE_CARLO,
    GUI_ID_TRANSPARENCY_SCROLL_BAR,
    GUI_ID_LABEL_RASTER_SCALE,
    GUI_ID_LABEL_IMAGES,
    GUI_ID_LABEL_BEST_SCORE,
    GUI_ID_LABEL_LOWEST_IMAGES,
    GUI_ID_LABEL_PROGRESS,
    GUI_ID_LABEL_ERROR_MESSAGE,
    GUI_ID_FACET_BUTTON,
    GUI_ID_POINTED_BUTTON,
    GUI_ID_HEXAGON_BUTTON,
    GUI_ID_SMALL_PUPIL_BUTTON,
    GUI_ID_POINTED_PUPIL_BUTTON,
    GUI_ID_SQUARE_BUTTON,
    GUI_ID_POINTED_TRIANGLE_BUTTON,
    GUI_ID_TRIANGLE_BUTTON,
    GUI_ID_TRAPEZE_BUTTON,
    GUI_ID_ELIPSE_BUTTON,
    GUI_ID_CARDOOR_BUTTON,
    GUI_ID_GRID_APPROACHES,
    GUI_ID_ALL
};

class MyEventReceiver : public EventReceiver
{
private:
    GraphicEngine* graphicEngine;

    BruteForceService* bruteForceService;
    BinaryService* binaryService;
    GreedyService* greedyService;
    GridApproachesService* gridApproachesService;
    MonteCarloService* monteCarloSerivce;
    MonteCarloHittingSetService* monteCarloRolloutSerivce;
    QuadSortService* quadSortService;

    Picture* picture;
    SAppContext* context;

    std::thread currentAlgorithmThread;
    std::thread currentSimulationThread;
	BananaPrimitive* primitive;

    bool isRunning;
    bool isAbbord;

    wchar_t progressSymbol;

    AGeometry* bananaFacet;
    AGeometry* pointFacet;
    AGeometry* smallPupilFacet;
    AGeometry* pointedPupilFacet;

    AGeometry* hexagon;
    AGeometry* square;
    AGeometry* pointedTriangle;
    AGeometry* triangle;
    AGeometry* trapeze;
    AGeometry* elipse;
    AGeometry* carDoor;

    double coverageOfLastRun;
    int imageCountOfLastRun;

public:
    MyEventReceiver(GraphicEngine* graphic_engine, BruteForceService* brute_force_service, GreedyService* greedy_service, GridApproachesService* gridApproachesService, MonteCarloHittingSetService* monte_carlo_hitting_set_service, 
        MonteCarloService* monte_carlo_service, QuadSortService* quad_sort_service, Picture* picture, BananaPrimitive* primitive, BinaryService* binary_service, AGeometry* geometry, 
        AGeometry* pointed_facet, AGeometry* hexagon, AGeometry* small_pointed_pupil_facet, AGeometry* pointed_pupil_facet, AGeometry* square, AGeometry* pointedTriangle, AGeometry* triangle, 
        AGeometry* trapeze, AGeometry* elipse, AGeometry* carDoor);

    ~MyEventReceiver();

    virtual void OnInit(SAppContext* context);

    void writeResults(const vector<SimulationResult>& vector);
    void startSimulation();
    void startAlgorithm(s32 algorithmId);
    bool setForm(s32 id);
    virtual bool OnEvent(const SEvent& event);

protected:
    void forceStepThread(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet, CalculationResult* result);
    void monteStepSecThread(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet, CalculationResult* result);
    void quadSearchThread(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet, CalculationResult* result);
    void monteStepRollThread(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet, CalculationResult* result);
    
private:
    void forceSolution(int maxThreadCount);
    void greedySolution();
    void quadsearch();
    void sequenceMonteCarlo(int maxThreadCount);
    void rolloutMonteCarlo(int maxThreadCount);
    void gridApproachesSolution();

    IGUIStaticText* addLabel(Point2D position, wstring text, double value);
    const wchar_t* getLabelText(wstring text, double value);

    void enableGUIElements(bool enable);

    void onAlgorithmStart();
    void onAlgorithmEnd();
    void onJoinTask();
    void onFoundSolution(int imageCount, double bestScore, int lowestImages);
    void onProgress();
};

#endif