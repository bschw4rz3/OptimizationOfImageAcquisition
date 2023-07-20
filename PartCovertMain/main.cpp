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

#include <iostream>
#include "../IrrlichtWrapper/GraphicEngine.h"

#include "BananaPrimitive.h"
#include "../PartCover/BananaFacet.h"
#include "../PartCover/PointedBananaFacet.h"
#include "MyEventReceiver.h"
#include "BruteForceService.h"
#include "GreedyService.h"
#include "MonteCarloHittingSetService.h"
#include "MonteCarloService.h"
#include "QuadSortService.h"
#include "../PartCover/Hexagon.h"
#include "../PartCover/PointedPupilFacet.h"
#include "../PartCover/Square.h"
#include "../PartCover/Triangle.h"
#include "../PartCover/Trapeze.h"
#include "../PartCover/Lens.h"

int main()
{
    QuadSortService::angleDetectionRadius = 0.65;
    QuadSortService::minAngle = 135;
    AGeometry::contourResolution = 0.5;
    RasterService::rasterResolution = 0.5;

    PointedPupilFacet pointedPupilFacet(16, 10);
    BananaFacet bananaFacet(24, 6, 15);
    PointedBananaFacet pointedFacet(24, 6, 15, 3, false);
    PointedPupilFacet smallPointedPupilFacet(7, 4);

    // Silvestermessung
    // Hexagon hexagon(16, 14); // 111.8 und 136.4 Grad // 184.8
    // Square square(16, 8); // 128
    // Triangle pointedTriangle(16, 8); // 64
    // Triangle triangle(7.8, 16); // 62.399
    Trapeze trapeze(7.8, 16); // 78
    // Lens elipse(24, 0, 4, 0, false); // 23.0006

    // Messung 14.01.
	Hexagon hexagon(10, 8); // 113.63 und 132.74 Grad // 66
	Square square(8, 16); // 128
	Triangle pointedTriangle(10, 10); // 50
	Triangle triangle(3, 5); // 7.5
	// Trapeze trapeze(8, 10); // 50
	// Lens elipse(30, 0, 7, 0, false); // 104.74233418387180

    // Messung 14.01. - Teil 2
	// Hexagon hexagon(4, 4); // 109.28.63 und 141.42 Grad // -13.199
    // Square square(5, 5); // 25
    // Trapeze trapeze(4, 5); // 12.5
    Lens elipse(26, 0, 5, 0, false); // 43.9335

    // Messung 14.01. - Teil 3
    // Hexagon hexagon(6, 4); // 117.7 und 124.6 Grad // 19.8
    // Square square(18, 18); // a=324
    // Triangle pointedTriangle(10, 10);// a=50
    // Triangle triangle(3, 5); // 7.5
    // Trapeze trapeze(4, 5); // 12.5
    // Lens elipse(22, 0, 3.5, 0, false); // 14.155

    double a = triangle.getArea();

    Picture picture(4.2, 2.8);
    picture.enableFocusPoints = true;
    picture.focusWidth = 0.5;

    RasterService rasterService;
    RasterManager rasterManager(&rasterService);
    MathHelper mathHelper;
    BinaryService binarySerivce;
    CalculationHelper calculationHelper(&mathHelper);

    BananaPrimitive primitive(&rasterService, &picture);
    primitive.position->x = 440;
    primitive.position->y = 280;
    primitive.scale = 15;
    
    BruteForceService bruteForceSerivce(&rasterManager, &mathHelper, &calculationHelper);
    GreedyService greedyService(&rasterManager, &mathHelper, &calculationHelper);
    MonteCarloService monteCarloSerivce(&rasterManager, &mathHelper, &calculationHelper);
    MonteCarloHittingSetService monteCarloRolloutSerivce(&rasterManager, &mathHelper, &calculationHelper);
    QuadSortService quadSortService(&rasterManager, &mathHelper, &calculationHelper);
    
    GraphicEngine graphicEngine;
    MyEventReceiver receiver(&graphicEngine, &bruteForceSerivce, &greedyService, &monteCarloRolloutSerivce, &monteCarloSerivce, &quadSortService, &picture, &primitive, &binarySerivce,
        (AGeometry*) &bananaFacet, (AGeometry*) &pointedFacet, (AGeometry*) &hexagon, (AGeometry*) &smallPointedPupilFacet, 
        (AGeometry*) &pointedPupilFacet, (AGeometry*) &square, (AGeometry*) &pointedTriangle, (AGeometry*) &triangle, (AGeometry*) &trapeze, 
        (AGeometry*) &elipse);

    graphicEngine.initiate(L"Part Cover", Point2D(640, 480));
    graphicEngine.loadFont(L"fonthaettenschweiler.bmp");

    graphicEngine.addButton(GUI_ID_FORCE, Point2D(0, 300), 80, L"Force", L"Calculate optimal result - may take a while...");
    graphicEngine.addButton(GUI_ID_GREEDY, Point2D(0, 330), 80, L"Greedy Algorithm", L"Solves the Problem with the Greedy Algorithm");
    graphicEngine.addButton(GUI_ID_SEQUENCE_MONTE_CARLO, Point2D(80, 300), 100, L"Sequence Monte Carlo", L"Sequence Monte Carlo");
    graphicEngine.addButton(GUI_ID_ROLLOUT_MONTE_CARLO, Point2D(80, 330), 100, L"Rollout Monte Carlo", L"Rollout Monte Carlo");
    graphicEngine.addButton(GUI_ID_QUADSEARCH, Point2D(0, 360), 100, L"Quadsearch", L"Quadsearch");
    graphicEngine.addButton(GUI_ID_ALL, Point2D(80, 360), 100, L"Simulate all", L"Startet unterschiedliche Formen mit allen Algrithmen");

    graphicEngine.addButton(GUI_ID_FACET_BUTTON, Point2D(80, 120), 60, L"Banana Facet", L"Banana Facet");
    graphicEngine.addButton(GUI_ID_POINTED_BUTTON, Point2D(80, 150), 60, L"Pointed Facet", L"Pointed Facet");
    graphicEngine.addButton(GUI_ID_HEXAGON_BUTTON, Point2D(0, 120), 60, L"Hexagon", L"Hexagon");
    graphicEngine.addButton(GUI_ID_SMALL_PUPIL_BUTTON, Point2D(0, 150), 75, L"Small Pupil Facet", L"Small Pupil Facet");
    graphicEngine.addButton(GUI_ID_POINTED_PUPIL_BUTTON, Point2D(0, 180), 75, L"Pointed Pupil Facet", L"Pointed Pupil Facet");
    graphicEngine.addButton(GUI_ID_SQUARE_BUTTON, Point2D(0, 210), 60, L"Square", L"Square");
    graphicEngine.addButton(GUI_ID_POINTED_TRIANGLE_BUTTON, Point2D(80, 210), 60, L"Pointed Triangle", L"Triangle");
    graphicEngine.addButton(GUI_ID_TRIANGLE_BUTTON, Point2D(80, 180), 60, L"Triangle", L"Triangle");
    graphicEngine.addButton(GUI_ID_TRAPEZE_BUTTON, Point2D(0, 240), 60, L"Trapez", L"Trapez");
    graphicEngine.addButton(GUI_ID_ELIPSE_BUTTON, Point2D(80, 240), 60, L"Ellipse", L"Ellipse");

    graphicEngine.addLabel(GUI_ID_LABEL_RASTER_SCALE, Point2D(150, 20), 100, L"Raster scale:");
    graphicEngine.addScrollbar(GUI_ID_TRANSPARENCY_SCROLL_BAR, Point2D(150, 45), 350, 0.01, 5.0, 1.0);

    graphicEngine.addLabel(GUI_ID_LABEL_IMAGES, Point2D(10, 10), 300, L"Images:");
    graphicEngine.addLabel(GUI_ID_LABEL_BEST_SCORE, Point2D(10, 30), 300, L"bestScore:");
    graphicEngine.addLabel(GUI_ID_LABEL_LOWEST_IMAGES, Point2D(10, 50), 300, L"lowestImages:");
    graphicEngine.addLabel(GUI_ID_LABEL_PROGRESS, Point2D(10, 70), 50, L"-");

    graphicEngine.addButton(GUI_ID_QUIT_BUTTON, Point2D(0, 420), 80, L"Quit", L"Exits Program");

    graphicEngine.addLabel(GUI_ID_LABEL_ERROR_MESSAGE, Point2D(300, 300), 300, L"");

    graphicEngine.addPrimitiveDraw((IPrimitiveDraw*) &primitive);

    graphicEngine.run((EventReceiver*) &receiver);
}
