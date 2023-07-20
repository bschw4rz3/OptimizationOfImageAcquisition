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

#ifndef GraphicEngine_H
#define GraphicEngine_H

#include <vector>
#include <map>
#include <ctime>

#include "irrlicht.h"
#include "driverChoice.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "SAppContext.h"
#include "Skin.h"
#include "Point2D.h"
#include "Color.h"
#include "IPrimitiveDraw.h"
#include "IPrimitiveGraphic.h"
#include "EventReceiver.h"
#include "IEventReceiver.h"

// Define some values that we'll use to identify individual GUI controls.
enum
{
    GUI_ID_QUIT_BUTTON = 1001,
    EET_TIMER_EVENT = 2001
};

class GraphicEngine : IPrimitiveGraphic
{
private:
    IrrlichtDevice* device;

    video::IVideoDriver* driver;
    IGUIEnvironment* env;

    std::vector<IPrimitiveDraw*> primitiveList;
    std::map<int, IGUIElement*> guiElementMap;

public:
    int initiate(const wchar_t* windowTitle, Point2D point);
    void loadFont(const wchar_t* fontFile);

    void addScrollbar(int id, Point2D position, int length, double min, double max, double value);
    void addButton(int id, Point2D position, int length, const wchar_t* text, const wchar_t* tooltip = L"");
    void addLabel(int id, Point2D position, int length, const wchar_t* text);
    void addListBox(int id, Point2D position, Point2D point, const wchar_t* text);
    void addInputBox(int id, Point2D point, int length, const wchar_t* text);

    void enableGUIElement(int id, bool enable);
    void setGUIElementText(int id, const wchar_t* text);

    void add2DLine(Point2D one, Point2D two, Color color);
    void add2DPixel(Point2D position, Color color);
    void add2DRectangle(Point2D from, Point2D to, Color color);
    void addImage(Point2D position, const wchar_t* file);

    void addPrimitiveDraw(IPrimitiveDraw* primitive);

    int run(EventReceiver* receiver);

private:
    void renderPrimitive(IGeometry* geometry);
};

#endif