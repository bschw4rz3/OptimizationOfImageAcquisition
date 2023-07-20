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

#include "VertexCoverConverter.h"

VertexCoverConverter::VertexCoverConverter(RasterManager* rasterManager)
{
	this->rasterManager = rasterManager;
}

std::vector<PointLink> VertexCoverConverter::convert(double rasterScale, AGeometry* facet, Picture* picture)
{
	std::vector<Point> sureface = this->rasterManager->getSurfaceRasterCash(rasterScale, facet);

	std::vector<PointLink> result = this->initPointLinks(sureface);
	std::map<Point, PointLink*> pointLinkMap = this->initLinkMap(&result);

	PointLink* currentLink = NULL;

	for(int i = 0; i < sureface.size() ;i++)
	{
		Point currentPoint = sureface[i];
		currentLink = pointLinkMap[currentPoint];
		
		std::vector<Point> covertPoints = this->rasterManager->calculateCoveredPoint(currentPoint, picture, facet, rasterScale);

		for(int childIndex = 0 ; childIndex < covertPoints.size(); childIndex++)
		{
			PointLink* childLink = pointLinkMap[covertPoints[childIndex]];
			currentLink->addLink(childLink);
		}
	}

	return result;
}

std::vector<PointLink> VertexCoverConverter::initPointLinks(std::vector<Point> sureface)
{
	std::vector<PointLink> result;

	for(int i = 0; i < sureface.size() ;i++)
	{
		std::ostringstream ss;
		ss << i;

		result.emplace_back(ss.str(), sureface[i]);
	}

	return result;
}

std::map<Point, PointLink*> VertexCoverConverter::initLinkMap(std::vector<PointLink>* pointLinks)
{
	std::map<Point, PointLink*> pointLinkMap;
	PointLink* linkArray = pointLinks->data();

	for(int i = 0; i < pointLinks->size() ;i++)
	{
		PointLink* link = (linkArray + i);
		const Point* point = link->getPoint();

		pointLinkMap.insert(std::pair<Point, PointLink*>(Point(point->x, point->y), link));
	}

	return pointLinkMap;
}
