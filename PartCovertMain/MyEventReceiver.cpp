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

#include "MyEventReceiver.h"

std::string enStr[] {
    stringify( GUI_ID_FORCE),
    stringify( GUI_ID_GREEDY),
    stringify( GUI_ID_QUADSEARCH),
    stringify( GUI_ID_SEQUENCE_MONTE_CARLO),
    stringify( GUI_ID_ROLLOUT_MONTE_CARLO),
    stringify( GUI_ID_TRANSPARENCY_SCROLL_BAR),
    stringify( GUI_ID_LABEL_RASTER_SCALE),
    stringify( GUI_ID_LABEL_IMAGES),
    stringify( GUI_ID_LABEL_BEST_SCORE),
    stringify( GUI_ID_LABEL_LOWEST_IMAGES),
    stringify( GUI_ID_LABEL_PROGRESS),
    stringify( GUI_ID_LABEL_ERROR_MESSAGE),
    stringify( GUI_ID_FACET_BUTTON),
    stringify( GUI_ID_POINTED_BUTTON),
    stringify( GUI_ID_HEXAGON_BUTTON),
    stringify( GUI_ID_SMALL_PUPIL_BUTTON),
    stringify( GUI_ID_POINTED_PUPIL_BUTTON),
    stringify( GUI_ID_SQUARE_BUTTON),
    stringify( GUI_ID_POINTED_TRIANGLE_BUTTON),
    stringify( GUI_ID_TRIANGLE_BUTTON),
    stringify( GUI_ID_TRAPEZE_BUTTON),
    stringify( GUI_ID_ELIPSE_BUTTON),
    stringify( GUI_ID_CARDOOR_BUTTON),
    stringify( GUI_ID_GRID_APPROACHES),
    stringify( GUI_ID_ALL )
};

MyEventReceiver::MyEventReceiver(GraphicEngine* graphicEngine, BruteForceService* bruteForceService, GreedyService* greedyService, GridApproachesService* gridApproachesService, MonteCarloHittingSetService* monteCarloRolloutService, 
                                 MonteCarloService* monteCarloSerivce,  QuadSortService* quadSortService, Picture* picture, BananaPrimitive* primitive, BinaryService* binaryService, AGeometry* bananaFacet, AGeometry* pointFacet,
                                 AGeometry* hexagon, AGeometry* smallPupilFacet, AGeometry* pointedPupilFacet, AGeometry* square, AGeometry* pointedTriangle, AGeometry* triangle, AGeometry* trapeze, AGeometry* elipse, AGeometry* carDoor)
{
    this->graphicEngine = graphicEngine;

    this->bruteForceService = bruteForceService;
    this->binaryService = binaryService;
    this->greedyService = greedyService;
    this->monteCarloSerivce = monteCarloSerivce;
    this->monteCarloRolloutSerivce = monteCarloRolloutService;
    this->quadSortService = quadSortService;
    this->gridApproachesService = gridApproachesService;

    this->facet = NULL;
    this->picture = picture;
    this->context = NULL;

    this->primitive = primitive;
    this->isRunning = false;
    this->isAbbord = false;

    this->progressSymbol = '-';

    this->bananaFacet = bananaFacet;
    this->pointFacet = pointFacet;
    this->smallPupilFacet = smallPupilFacet;
    this->pointedPupilFacet = pointedPupilFacet;

    this->hexagon = hexagon;
    this->square = square;
    this->pointedTriangle = pointedTriangle;
    this->triangle = triangle;
    this->trapeze = trapeze;
    this->elipse = elipse;
    this->carDoor = carDoor;
}

MyEventReceiver::~MyEventReceiver()
{
    this->isAbbord = true;

    if (this->isRunning)
    {
        this->onJoinTask();
    }
}

void MyEventReceiver::OnInit(SAppContext* context)
{
    this->context = context;
}

void MyEventReceiver::writeResults(const vector<SimulationResult>& vector)
{
	std::ofstream myfile;
    
    myfile.open("results.csv");
	myfile << "Algorithmus;Geometrie;Zeit(ms);Bilderanzahl;Abdeckungsrate\n";

	for (auto result : vector)
	{
        myfile << result.algorithmName << ";" << result.form << ";" << result.millisecond << ";" << result.countOfImages << ";" << result.coverage << "\n";
	}

    myfile.close();

}

void MyEventReceiver::startSimulation()
{
    std::vector<s32> formIdList{ /*GUI_ID_ELIPSE_BUTTON, GUI_ID_POINTED_TRIANGLE_BUTTON, GUI_ID_HEXAGON_BUTTON, GUI_ID_TRIANGLE_BUTTON,
    	GUI_ID_TRAPEZE_BUTTON, GUI_ID_SQUARE_BUTTON,*/ GUI_ID_CARDOOR_BUTTON  };

	std::vector<s32> algorithmIdList{GUI_ID_QUADSEARCH, GUI_ID_GREEDY, GUI_ID_FORCE, GUI_ID_SEQUENCE_MONTE_CARLO, GUI_ID_ROLLOUT_MONTE_CARLO, GUI_ID_GRID_APPROACHES };

    std::vector<SimulationResult> result;
    
    for (int formId : formIdList)
    {
	    for(int algId : algorithmIdList)
	    {
		   int secondsCounter = 0;

            auto start = chrono::steady_clock::now();
            long long executionTime = 0;

            this->setForm(formId);
			this->startAlgorithm(algId);

            unsigned long long sleepIntervall = 1;
            unsigned long long maxCounts = 10000/sleepIntervall;

            while(this->isRunning && secondsCounter < maxCounts && executionTime < 300000)
            {
	            std::this_thread::sleep_for(sleepIntervall*1ms);

                auto end = chrono::steady_clock::now();
                executionTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();

				secondsCounter++;
            }

            auto end = chrono::steady_clock::now();
            executionTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();

            result.push_back(SimulationResult(enStr[algId], enStr[formId], executionTime, this->imageCountOfLastRun, this->coverageOfLastRun));

            this->isAbbord = true;
            this->onJoinTask();
            this->isAbbord = false;

            this->monteCarloRolloutSerivce->reset();
            this->quadSortService->reset();
            this->greedyService->reset();
            this->gridApproachesService->reset();
	    }
    }

    this->writeResults(result);
}

void MyEventReceiver::startAlgorithm(s32 algorithmId)
{
    this->onJoinTask();

    this->monteCarloRolloutSerivce->reset();
    this->quadSortService->reset();
    this->greedyService->reset();
    this->gridApproachesService->reset();

	if (algorithmId == GUI_ID_GREEDY)
	{
		this->currentAlgorithmThread = std::thread(&MyEventReceiver::greedySolution, this);
	}
	else if (algorithmId == GUI_ID_FORCE)
	{
		this->currentAlgorithmThread = std::thread(&MyEventReceiver::forceSolution, this, 8);
	}
	else if (algorithmId == GUI_ID_SEQUENCE_MONTE_CARLO)
	{
		this->currentAlgorithmThread = std::thread(&MyEventReceiver::sequenceMonteCarlo, this, 8);
	}
	else if (algorithmId == GUI_ID_ROLLOUT_MONTE_CARLO)
	{
		this->currentAlgorithmThread = std::thread(&MyEventReceiver::rolloutMonteCarlo, this, 8);
	}
	else if (algorithmId == GUI_ID_QUADSEARCH)
	{
		this->currentAlgorithmThread = std::thread(&MyEventReceiver::quadsearch, this);
	}
	else if (algorithmId == GUI_ID_GRID_APPROACHES)
	{
		this->currentAlgorithmThread = std::thread(&MyEventReceiver::gridApproachesSolution, this);
	}
	else if(algorithmId == GUI_ID_ALL)
	{
        this->currentSimulationThread = std::thread(&MyEventReceiver::startSimulation, this);
        //this->startSimulation();
	}
    else
    {
	    throw "unknow Alg...";
    }

	this->onAlgorithmStart();
}

bool MyEventReceiver::setForm(s32 id)
{
	switch (id) {  
	    case GUI_ID_FACET_BUTTON:
	        this->facet = (IGeometry*) this->bananaFacet;
	        return true;
	    case GUI_ID_POINTED_BUTTON:
	        this->facet = (IGeometry*) this->pointFacet;
	        return true;
	    case GUI_ID_HEXAGON_BUTTON:
	        this->facet = (IGeometry*) this->hexagon;
	        return true;
	    case GUI_ID_SMALL_PUPIL_BUTTON:
	        this->facet = (IGeometry*) this->smallPupilFacet;
	        return true;
	    case GUI_ID_POINTED_PUPIL_BUTTON:
	        this->facet = (IGeometry*) this->pointedPupilFacet;
	        return true;
		case GUI_ID_SQUARE_BUTTON:
	        this->facet = (IGeometry*) this->square;
	        return true;
		case GUI_ID_POINTED_TRIANGLE_BUTTON:
	        this->facet = (IGeometry*) this->pointedTriangle;
	        return true;
		case GUI_ID_TRIANGLE_BUTTON:
	        this->facet = (IGeometry*) this->triangle;
	        return true;
		case GUI_ID_TRAPEZE_BUTTON:
	        this->facet = (IGeometry*) this->trapeze;
	        return true;
		case GUI_ID_ELIPSE_BUTTON:
	        this->facet = (IGeometry*) this->elipse;
	        return true;
        case GUI_ID_CARDOOR_BUTTON:
	        this->facet = (IGeometry*) this->carDoor;
	        return true;
	    default:
	        return false;
    }
}

bool MyEventReceiver::OnEvent(const SEvent& event)
{
    if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();

        IGUIEnvironment* env = this->context->device->getGUIEnvironment();

        switch (event.GUIEvent.EventType)
        {
        case EGET_SCROLL_BAR_CHANGED:
            if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
            {
                s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
                this->primitive->rasterScale = pos;
            }
            break;
        case EGET_BUTTON_CLICKED:
            switch (id)
            {
            case GUI_ID_QUIT_BUTTON:
                this->isAbbord = true;
                this->onJoinTask();
                this->context->device->closeDevice();
                return true;
            case GUI_ID_FORCE:
            case GUI_ID_GREEDY:
            case GUI_ID_SEQUENCE_MONTE_CARLO:
            case GUI_ID_ROLLOUT_MONTE_CARLO:
            case GUI_ID_QUADSEARCH:
			case GUI_ID_GRID_APPROACHES:
			case GUI_ID_ALL:
            {
                if (!this->isRunning)
                {
                    this->startAlgorithm(id);
                }
                else
                {
                    rect<s32> size(100 + this->context->counter, 100 + this->context->counter, 300 + this->context->counter, 200 + this->context->counter);

                    IGUIWindow* window = env->addWindow(
                        size,
                        false, // modal?
                        L"Process blocked!");

                    env->addStaticText(L"There is allready a process running!",
                        rect<s32>(0, 35, 200, 50),
                        true, // border?
                        false, // wordwrap?
                        window);
                }
            }

            return true;

            default:
                return this->setForm(id);
            }
            break;

        case EGET_FILE_SELECTED:
        {
            // show the model filename, selected in the file dialog
            IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
            //Context.listbox->addItem(dialog->getFileName());
        }
        break;

        default:
            break;
        }
    }
    else if (event.EventType == EET_GUI_EVENT)
    {
        if (!this->isRunning)
        {
            this->onJoinTask();
        }
    }

    return false;
}

void MyEventReceiver::forceStepThread(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet, CalculationResult* result)
{
    CalculationResult tempResult = this->bruteForceService->DoCalculationStep(binaryCombination, rasterScale, picture, facet);

    result->coverRate = tempResult.coverRate;
    result->imageCount = tempResult.imageCount;
    result->coveredPoints = tempResult.coveredPoints;
    result->binaryCombination = tempResult.binaryCombination;
}

void MyEventReceiver::monteStepSecThread(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet, CalculationResult* result)
{
    std::string newBinaryCombination = this->monteCarloSerivce->mutateCombination(binaryCombination, rasterScale, picture, facet);
    CalculationResult tempResult = this->monteCarloSerivce->calculateScore(newBinaryCombination, rasterScale, picture, facet);

    result->coverRate = tempResult.coverRate;
    result->imageCount = tempResult.imageCount;
    result->coveredPoints = tempResult.coveredPoints;
    result->binaryCombination = newBinaryCombination;
}

void MyEventReceiver::monteStepRollThread(std::string binaryCombination, double rasterScale, Picture* picture, AGeometry* facet, CalculationResult* result)
{
    CalculationResult tempResult = this->monteCarloRolloutSerivce->calculateStep(binaryCombination, rasterScale, picture, facet);
    
    result->coverRate = tempResult.coverRate;
    result->imageCount = tempResult.imageCount;
    result->coveredPoints = tempResult.coveredPoints;
}

void MyEventReceiver::quadsearch()
{
    double rasterScale = this->primitive->rasterScale;

    double bestScore = 0;
    int lowestImages = 9999999999999;
    std::vector<Point> bestCoveredPoints;

    std::vector<Point> resultList;
    std::vector<Point> possibleStartPoints;
    CalculationQuadResult result;

    long currentIndex = 0;

    while (result.coverRate != 1.0 && !this->isAbbord)
    {
        try
        {
            result = this->quadSortService->DoCalculationStep(resultList, currentIndex, possibleStartPoints, rasterScale, picture, (AGeometry*) facet);
        }
        catch (std::exception& e)
        {
            std::string s(e.what());
            std::wstring errorMessage(s.begin(), s.end());

            this->graphicEngine->setGUIElementText(GUI_ID_LABEL_ERROR_MESSAGE, errorMessage.c_str());
            break;  
        }

        if(result.coverRate > bestScore || (bestScore == result.coverRate && result.coverRate == 0))
        {
	        resultList = result.coveredPoints;

	        bestCoveredPoints = result.coveredPoints;
	        bestScore = result.coverRate;
	        lowestImages = result.imageCount;

			this->onFoundSolution(bestCoveredPoints.size(), bestScore, lowestImages);
			this->primitive->setOptimalSolution(resultList, this->quadSortService->contourMarker, this->quadSortService->corners);
        }

        possibleStartPoints = result.startPoints;

        this->onProgress();

        currentIndex++;
    }

    this->isRunning = false;
}

void MyEventReceiver::greedySolution()
{
    double rasterScale = this->primitive->rasterScale;

    double bestScore = 0;
    int lowestImages = 9999999999999;
    std::vector<Point> bestCoveredPoints;

    std::vector<Point> resultList;
    CalculationResult result;

    while (result.coverRate != 1.0 && !this->isAbbord)
    {
        try
        {
            result = this->greedyService->DoCalculationStep(resultList, rasterScale, picture, (AGeometry*) this->facet);
        }
        catch (std::exception& e)
        {
            std::string s(e.what());
            std::wstring errorMessage(s.begin(), s.end());

            this->graphicEngine->setGUIElementText(GUI_ID_LABEL_ERROR_MESSAGE, errorMessage.c_str());
            break;
        }
        
        resultList = result.coveredPoints;

        bestCoveredPoints = result.coveredPoints;
        bestScore = result.coverRate;
        lowestImages = result.imageCount;

        this->onFoundSolution(bestCoveredPoints.size(), bestScore, lowestImages);
        this->primitive->setOptimalSolution(resultList);

        this->onProgress();
    }

    this->isRunning = false;
}

void MyEventReceiver::gridApproachesSolution()
{
    double rasterScale = this->primitive->rasterScale;

    double bestScore = 0;
    int lowestImages = 9999999999999;
    std::vector<Point> bestCoveredPoints;

    std::vector<Point> resultList;
    CalculationResult result;

    while (result.coverRate != 1.0 && !this->isAbbord)
    {
        try
        {
            result = this->gridApproachesService->DoCalculationStep(resultList, rasterScale, picture, (AGeometry*) this->facet);
        }
        catch (std::exception& e)
        {
            std::string s(e.what());
            std::wstring errorMessage(s.begin(), s.end());

            this->graphicEngine->setGUIElementText(GUI_ID_LABEL_ERROR_MESSAGE, errorMessage.c_str());
            break;
        }
        
        resultList = result.coveredPoints;

        bestCoveredPoints = result.coveredPoints;
        bestScore = result.coverRate;
        lowestImages = result.imageCount;

        this->onFoundSolution(bestCoveredPoints.size(), bestScore, lowestImages);
        this->primitive->setOptimalSolution(resultList);

        this->onProgress();
    }

    this->isRunning = false;
}

void MyEventReceiver::forceSolution(int maxThreadCount)
{
    double rasterScale = this->primitive->rasterScale;

    std::string binaryCombination = this->bruteForceService->generateFirstBinaryCombination(rasterScale, (AGeometry*) this->facet);
    std::string lastBinaryCombination = this->bruteForceService->generateLastBinaryCombination(rasterScale, (AGeometry*) this->facet);

    double bestScore = 0;
    int lowestImages = 9999999999999;
    std::vector<Point> bestCoveredPoints;

    std::vector<std::thread> threadList(maxThreadCount);
    std::vector<CalculationResult*> resultList(maxThreadCount);

    while (binaryCombination != lastBinaryCombination && !this->isAbbord)
    {
        // Create some new threads
        for (int threadindex = 0; threadindex < maxThreadCount; threadindex++)
        {
            resultList[threadindex] = new CalculationResult();

            threadList[threadindex] = std::thread(&MyEventReceiver::forceStepThread, this, binaryCombination, rasterScale, picture, (AGeometry*) this->facet, resultList[threadindex]);
            binaryCombination = this->binaryService->raise(binaryCombination);
        }

        // Wait for result 
        for (int threadindex = 0; threadindex < threadList.size(); threadindex++)
        {
            threadList[threadindex].join();
            CalculationResult* result = resultList[threadindex];

            if (bestScore < result->coverRate || (bestScore <= result->coverRate && lowestImages > result->imageCount))
            {
                bestCoveredPoints = result->coveredPoints;
                bestScore = result->coverRate;
                lowestImages = result->imageCount;

                this->onFoundSolution(bestCoveredPoints.size(), bestScore, lowestImages);
                this->primitive->setOptimalSolution(bestCoveredPoints);
            }

            delete resultList[threadindex];
        }

        this->onProgress();
    }

    this->isRunning = false;
}

void MyEventReceiver::sequenceMonteCarlo(int maxThreadCount)
{
    double rasterScale = this->primitive->rasterScale;

    std::string bestBinaryCombination = this->monteCarloSerivce->generateRandomtBinaryCombination(rasterScale, (AGeometry*) facet);
    CalculationResult tempResult = this->monteCarloSerivce->calculateScore(bestBinaryCombination, rasterScale, picture, (AGeometry*) facet);

    double bestScore = tempResult.coverRate;
    int lowestImages = tempResult.imageCount;
    std::vector<Point> bestCoveredPoints = tempResult.coveredPoints;

    std::vector<std::thread> threadList(maxThreadCount);
    std::vector<CalculationResult*> resultList(maxThreadCount);

    while (bestScore <= 1 && !this->isAbbord)
    {
        // Create some new threads
        for (int threadindex = 0; threadindex < maxThreadCount; threadindex++)
        {
            resultList[threadindex] = new CalculationResult();
            threadList[threadindex] = std::thread(&MyEventReceiver::monteStepSecThread, this, bestBinaryCombination, rasterScale, picture, (AGeometry*) facet, resultList[threadindex]);
        }

        // Wait for result 
        for (int threadindex = 0; threadindex < threadList.size(); threadindex++)
        {
            threadList[threadindex].join();
            CalculationResult* result = resultList[threadindex];

            if (bestScore < result->coverRate || (bestScore <= result->coverRate && lowestImages > result->imageCount))
            {
                bestCoveredPoints = result->coveredPoints;
                bestScore = result->coverRate;
                lowestImages = result->imageCount;

                this->onFoundSolution(bestCoveredPoints.size(), bestScore, lowestImages);
                this->primitive->setOptimalSolution(bestCoveredPoints);
            }

            delete resultList[threadindex];
        }

        this->onProgress();
    }

    this->isRunning = false;
}

void MyEventReceiver::rolloutMonteCarlo(int maxThreadCount)
{
    double bestScore = 0;
    int lowestImages = 9999999999999;
    std::vector<Point> bestCoveredPoints;

    std::vector<Point> resultList;
    CalculationResult result;

    double rasterScale = this->primitive->rasterScale;
    
    std::string bestBinaryCombination = this->monteCarloRolloutSerivce->generateEmptyBinaryCombination(rasterScale, (AGeometry*) facet);
    CalculationResult tempResult = this->monteCarloRolloutSerivce->calculateStep(bestBinaryCombination, rasterScale, picture, (AGeometry*) facet);

    bestScore = result.coverRate;
    lowestImages = result.imageCount;
    bestCoveredPoints = result.coveredPoints;

    int currentIndex = 0;

    while (result.coverRate != 1.0 && !this->isAbbord)
    {
        try
        {
            result = this->monteCarloRolloutSerivce->calculateStep(bestBinaryCombination, rasterScale, picture, (AGeometry*) facet);
        }
        catch (std::exception& e)
        {
            std::string s(e.what());
            std::wstring errorMessage(s.begin(), s.end());

            this->graphicEngine->setGUIElementText(GUI_ID_LABEL_ERROR_MESSAGE, errorMessage.c_str());
            break;  
        }

        if(result.coverRate > bestScore)
        {
	        resultList = result.coveredPoints;

	        bestCoveredPoints = result.coveredPoints;
	        bestScore = result.coverRate;
	        lowestImages = result.imageCount;
	        bestBinaryCombination = result.binaryCombination;

			this->onFoundSolution(bestCoveredPoints.size(), bestScore, lowestImages);
			this->primitive->setOptimalSolution(resultList, this->quadSortService->contourMarker, this->quadSortService->corners);
        }

        this->onProgress();

        currentIndex++;
    }

    this->isRunning = false;
}

const wchar_t* MyEventReceiver::getLabelText(wstring text, double value)
{
    std::wostringstream myStringStream;
    myStringStream << text << value;

    const wchar_t* result = myStringStream.str().c_str();
    return result;
}

IGUIStaticText* MyEventReceiver::addLabel(Point2D position, wstring text, double value)
{
    IGUIEnvironment* env = this->context->device->getGUIEnvironment();
    const wchar_t* totalText = this->getLabelText(text, value);

    return env->addStaticText(totalText, recti(position.x, position.y, position.x + 200, position.y + 32), true);
}

void MyEventReceiver::onAlgorithmStart()
{
    this->isRunning = true;
    this->enableGUIElements(false);
}

void MyEventReceiver::onAlgorithmEnd()
{
    this->isRunning = false;
    this->enableGUIElements(true);
}

void MyEventReceiver::onJoinTask()
{
    if (this->currentAlgorithmThread.joinable())
    {
        this->currentAlgorithmThread.join();
    }
}

void MyEventReceiver::onFoundSolution(int imageCount, double bestScore, int lowestImages)
{
    if (!this->isAbbord && this->isRunning)
    {
        this->coverageOfLastRun = bestScore;
        this->imageCountOfLastRun = imageCount;

        double roundedBestScore = (round(bestScore * 10000.0) / 10000.0);

        wstring imageText = L"Images: " + std::to_wstring(imageCount);
        wstring bestScoreText = L"bestScore: " + std::to_wstring(roundedBestScore);
        wstring lowestImagesText = L"lowestImages: " + std::to_wstring(lowestImages);

        this->graphicEngine->setGUIElementText(GUI_ID_LABEL_IMAGES, imageText.c_str());
        this->graphicEngine->setGUIElementText(GUI_ID_LABEL_BEST_SCORE, bestScoreText.c_str());
        this->graphicEngine->setGUIElementText(GUI_ID_LABEL_LOWEST_IMAGES, lowestImagesText.c_str());
    }
}

void MyEventReceiver::onProgress()
{
    if (this->progressSymbol == '-')
    {
        this->progressSymbol = '\\';
    }
    else if(this->progressSymbol == '\\')
    {
        this->progressSymbol = '/';
    }
    else if (this->progressSymbol == '/')
    {
        this->progressSymbol = '-';
    }

    std::wstring progressString(1, this->progressSymbol);

    this->graphicEngine->setGUIElementText(GUI_ID_LABEL_PROGRESS, progressString.c_str());
}

void MyEventReceiver::enableGUIElements(bool enable)
{
    /*
    this->graphicEngine->enableGUIElement(GUI_ID_FORCE, enable);
    this->graphicEngine->enableGUIElement(GUI_ID_GREEDY, enable);
    this->graphicEngine->enableGUIElement(GUI_ID_SEQUENCE_MONTE_CARLO, enable);
    this->graphicEngine->enableGUIElement(GUI_ID_TRANSPARENCY_SCROLL_BAR, enable);
    */
}