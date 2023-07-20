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
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS �AS IS� AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
// DAMAGE.
// 
// Parts of this Software are based on Irrlicht Engine, zlib, libpng and IJG-Code

#include "Picture.h"

Picture::Picture(double width, double height)
{
	this->width = width;
	this->height = height;

	this->focusWidth = 0;
	this->focusHeight = 0;
}

std::vector<Point> Picture::getRelativFocusPoints()
{
	std::vector<Point> result;

	if(this->focusWidth == 0 && this->focusHeight != 0)
	{
		result.push_back(Point(0,-this->focusHeight));
		result.push_back(Point(0,+this->focusHeight));
	}
	else if(this->focusWidth != 0 && this->focusHeight == 0)
	{
		result.push_back(Point(-this->focusWidth,0));
		result.push_back(Point(+this->focusWidth,0));
	}
	else if(this->focusWidth == 0 && this->focusHeight == 0)
	{
		result.push_back(Point(-this->focusWidth,-this->focusHeight));
		result.push_back(Point(-this->focusWidth,+this->focusHeight));
		result.push_back(Point(+this->focusWidth,-this->focusHeight));
		result.push_back(Point(+this->focusWidth,+this->focusHeight));
	}
	else
	{
		result.push_back(Point(0,0));
	}

	return result;
}